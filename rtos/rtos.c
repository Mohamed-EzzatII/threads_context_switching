#include"rtos.h"



/*
 * Lists to represent the thread_id of the running and stopped threads.
 * NOTE:
 * The Initialization by zero is available in linux and windows only because
 * pthread_t datatype is OS dependant according to the POSIX Standard
 * you can change the initialization value in the threads_linked_list.h file
 */

static LinkedList stopped_threads;
static LinkedList running_threads;

/* array to store the created threads initially */
pthread_t threads[NUMBER_OF_THREADS];
pthread_t main_thread;

/*
 * Mutex to ensure that the kernel code won't be interrupted
 * by a thread
 */
static pthread_mutex_t kernel_mutex = PTHREAD_MUTEX_INITIALIZER;


/*
 * Condition Variable and mutex to sync the main
 */
static pthread_cond_t main_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t main_mutex = PTHREAD_MUTEX_INITIALIZER;



static __uint8_t signal_is_handled = SIGNAL_UNHANDLED;

static void increment_tick(int sig);
static void stop_thread_handler(int sig);
static void resume_thread_handler(int sig);
static void pthread_body(void *arg);
static void increment_tick(int sig);

void stop_thread_handler(int sig)
{
    sigset_t signal_mask;
    sigfillset(&signal_mask);
    sigdelset(&signal_mask, SIGUSR2);
    sigdelset(&signal_mask, SIGALRM);
    printf("signal %lld is paused\n",pthread_self());
    signal_is_handled = SIGNAL_HANDLED;
    sigsuspend(&signal_mask);
    return;
}

/*
 * This handler is called once SIGUSR2 signal is fired
 * all what it does is nothing, but we must define the
 * handler of SIGUSR2
 */
void resume_thread_handler(int sig)
{
    return;
}

/*
 * send signal SIGUSR1 to stop the thread execution
 */
int stop_thread(pthread_t thread_to_stop)
{
    /* Variable to store the status of each syscall or library call */
    int status;
    /* Try to lock the kernel mutex*/
    if (pthread_mutex_lock(&kernel_mutex) != 0)
    {
        printf("Can not lock the kernel mutex to stop thread\n");
        return ERROR;
    }
    /* Search for the thread */
    if (search_node(&stopped_threads, thread_to_stop) != NULL)
    {
        printf("thread Is already stopped\n");
        pthread_mutex_unlock(&kernel_mutex);
        return ERROR;
    }

    /* Try to send the stop signal to the last empty thread */
    if (pthread_kill(thread_to_stop, SIGUSR1) != 0)
    {
        printf("Can not send stop signal\n");
        pthread_mutex_unlock(&kernel_mutex);
        return ERROR;
    }

    /* wait until the signal is handled*/
    while (signal_is_handled == SIGNAL_UNHANDLED);
    signal_is_handled = SIGNAL_UNHANDLED;
    if (thread_to_stop != main_thread)
    {
        /* add the stopped thread to the list*/
        add_node_end(&stopped_threads, thread_to_stop);

        /* remove it from the running list */
        delete_node(&running_threads, thread_to_stop);

        /* Try to unlock the kernel mutex*/
        if (pthread_mutex_unlock(&kernel_mutex) != 0)
        {
            printf("Can not unlock the kernel mutex after stopping thread\n");
            return ERROR;
        }
    }
    else{
        /* Try to unlock the kernel mutex*/
        if (pthread_mutex_unlock(&kernel_mutex) != 0)
        {
            printf("Can not unlock the kernel mutex after stopping thread\n");
            return ERROR;
        }
        pthread_cond_wait(&main_cond, &main_mutex); // Main thread waits here
    }
    return !ERROR;
}

/*
 * send signal SIGUSR2 to resume the thread execution
 */
int resume_thread(pthread_t thread_to_resume)
{
    /* Variable to store the status of each syscall or library call */
    int status;

    /* Try to lock the kernel mutex*/
    if (pthread_mutex_lock(&kernel_mutex) != 0)
    {
        printf("Can not lock the kernel mutex to resume thread\n");
        return ERROR;
    }
    /* Search for the thread */
    if (search_node(&running_threads, thread_to_resume) != NULL)
    {
        printf("thread Is already running\n");
        pthread_mutex_unlock(&kernel_mutex);
        return ERROR;
    }

    /* Try to send the resume signal to the last empty thread */
    if (pthread_kill(thread_to_resume, SIGUSR2) != 0)
    {
        printf("Can not send resume signal\n");
        pthread_mutex_unlock(&kernel_mutex);
        return ERROR;
    }
    if (thread_to_resume != main_thread)
    {
        /* add the resumed thread to the list*/
        add_node_end(&running_threads, thread_to_resume);

        /* remove it from the stopped list */
        delete_node(&stopped_threads, thread_to_resume);
    }
    else{
        pthread_cond_signal(&main_cond);
        pthread_mutex_unlock(&main_mutex); // Unlock mutex after wait is over
    }
    if (pthread_mutex_unlock(&kernel_mutex) != 0)
    {
        printf("Can not unlock the kernel mutex after resuming thread\n");
        return ERROR;
    }

    return !ERROR;
}

void pthread_body(void *arg)
{
    sleep(1);
    int pthread_number = (int)arg;
    for (int i = 1; i < 100; i++)
    {
        printf("Thread[%d] is running in iteration[%d]\n", pthread_number, i);
    }
    printf("Thread[%d] finished\n");
    sleep(1);
    resume_thread(main_thread);
}

/* Initialize the signals */
void init_signals()
{

    struct sigaction sigusr1, sigusr2;

    sigusr1.sa_flags = 0;
    sigusr1.sa_handler = stop_thread_handler;

    sigemptyset(&sigusr1.sa_mask);
    sigusr2.sa_flags = 0;
    sigusr2.sa_handler = resume_thread_handler;
    sigusr2.sa_mask = sigusr1.sa_mask;

    if (sigaction(SIGUSR1, &sigusr1, NULL) == -1)
    {
        printf(" Error in Init the stop handler\n");
    }

    if (sigaction(SIGUSR2, &sigusr2, NULL) == -1)
    {
        printf(" Error in Init the resume handler\n");
    }
}

void init_threads()
{

    /* Initialize the lists */
    init_list(&running_threads);
    init_list(&stopped_threads);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    struct sched_param param;
    param.sched_priority = 0;
    int policy = SCHED_RR;
    pthread_attr_setschedparam(&attr,&param);
    pthread_attr_setschedpolicy(&attr,&policy);
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        if (pthread_create(&threads[i], &attr, pthread_body, (void *)i) != 0)
        {
            printf("Error in creating thread[%d]\n", i);

        }
        else{
            /* add the running thread to the list*/
            add_node_end(&running_threads, threads[i]);
        }
    }

    /* add all the threads in the running state initially */
}

void increment_tick(int sig)
{
    // tick++;
    return;
}