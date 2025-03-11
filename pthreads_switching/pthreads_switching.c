/**
 * @file context_switching.c
 * @brief Implementation of a simple mechanism for switching between pthreads.
 * @author Mohamed Ezzat
 * @date 2025-03-11
 * @version v1.0.1
 */

/*******************************************************************
 * Includes
 *******************************************************************/
 #include "pthreads_switching.h"

/*******************************************************************
 * Static Global Variables
 *******************************************************************/
/**
 * @brief List of threads that are currently stopped.
 * @details This list stores the thread IDs of threads that have been paused using the `stop_thread` function.
 */
static LinkedList stopped_threads;

/**
 * @brief List of threads that are currently running.
 * @details This list stores the thread IDs of threads that are actively executing.
 */
static LinkedList running_threads;

/**
 * @brief Mutex to ensure that the kernel code won't be interrupted by a thread.
 * @details This mutex is used to protect critical sections of the kernel code.
 */
static pthread_mutex_t kernel_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Mutex for synchronizing the main thread.
 * @details This mutex is used to control access to the main thread's critical sections.
 */
static pthread_mutex_t main_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Condition variable for synchronizing the main thread.
 * @details This condition variable is used to signal the main thread when it needs to resume.
 */
static pthread_cond_t main_cond = PTHREAD_COND_INITIALIZER;

/**
 * @brief Flag to indicate if a signal has been handled.
 * @details This flag is used to synchronize signal handling between threads.
 */
static __uint8_t signal_is_handled = SIGNAL_UNHANDLED;
/*******************************************************************
 * Global Variables
 *******************************************************************/

/**
 * @brief Array to store the created threads initially.
 * @details This array holds the thread IDs of all threads created by the system.
 */
pthread_t threads[NUMBER_OF_THREADS];

/**
 * @brief Stores the thread ID of the main thread.
 * @details This variable is used to identify the main thread for special handling.
 */
pthread_t main_thread;

/*******************************************************************
 * Static Functions
 *******************************************************************/
/**
 * @brief Increment the system tick (unused in this implementation).
 * @param sig The signal number (unused).
 */
static void increment_tick(int sig);

/**
 * @brief Signal handler for SIGUSR1, which stops the thread execution.
 * @param sig The signal number (unused).
 */
static void stop_thread_handler(int sig);

/**
 * @brief Signal handler for SIGUSR2, which resumes the thread execution.
 * @param sig The signal number (unused).
 */
static void resume_thread_handler(int sig);

/**
 * @brief Entry point for created threads.
 * @param arg The argument passed to the thread (unused).
 */
static void pthread_body(void *arg);
/**
 * @brief Signal handler for SIGUSR1, which stops the thread execution.
 * @param sig The signal number (unused).
 * @details This handler blocks all signals except SIGUSR2 and SIGALRM, then suspends the thread until a resume signal is received.
 */

/*******************************************************************
 * Signal Handlers
 *******************************************************************/

/**
 * @brief Signal handler for SIGUSR1, which stops the thread execution.
 * @param sig The signal number (unused).
 * @details This handler blocks all signals except SIGUSR2 and SIGALRM, then suspends the thread until a resume signal is received.
 */
void stop_thread_handler(int sig)
{
    (void) sig; /* To remove warning */
    sigset_t signal_mask;
    sigfillset(&signal_mask);  /* Block all signals */
    sigdelset(&signal_mask, SIGUSR2);  /* Unblock SIGUSR2 */
    sigdelset(&signal_mask, SIGALRM);  /* Unblock SIGALRM */
    signal_is_handled = SIGNAL_HANDLED;  /* Mark the signal as handled */
    sigsuspend(&signal_mask);  /* Suspend the thread until a resume signal is received */
    return;
}

/**
 * @brief Signal handler for SIGUSR2, which resumes the thread execution.
 * @param sig The signal number (unused).
 * @details This handler is called when SIGUSR2 is received. It does nothing but must be defined to handle the signal.
 */
void resume_thread_handler(int sig)
{

    (void) sig; /* To remove warning */
    return;
}

/*******************************************************************
 * Functions
 *******************************************************************/

/**
 * @brief Sends a SIGUSR1 signal to stop the execution of a thread.
 * @param thread_to_stop The thread ID of the thread to be stopped.
 * @return Returns `!ERROR` on success, `ERROR` on failure.
 * @details This function sends a SIGUSR1 signal to the specified thread, which triggers the `stop_thread_handler` to pause the thread.
 *          The thread is then moved from the `running_threads` list to the `stopped_threads` list.
 */
int stop_thread(pthread_t thread_to_stop)
{
    /* Variable to store the status of each syscall or library call */
    int status;

    /* Try to lock the kernel mutex */
    if (pthread_mutex_lock(&kernel_mutex) != 0)
    {
        printf("Cannot lock the kernel mutex to stop thread\n");
        return ERROR;
    }

    /* Search for the thread in the stopped_threads list */
    if (search_node(&stopped_threads, thread_to_stop) != NULL)
    {
        printf("Thread is already stopped\n");
        pthread_mutex_unlock(&kernel_mutex);
        return ERROR;
    }

    /* Try to send the stop signal (SIGUSR1) to the thread */
    if (pthread_kill(thread_to_stop, SIGUSR1) != 0)
    {
        printf("Cannot send stop signal\n");
        pthread_mutex_unlock(&kernel_mutex);
        return ERROR;
    }

    /* Wait until the signal is handled */
    while (signal_is_handled == SIGNAL_UNHANDLED);
    signal_is_handled = SIGNAL_UNHANDLED;

    /* If the thread is not the main thread, update the lists */
    if (thread_to_stop != main_thread)
    {
        /* Add the stopped thread to the stopped_threads list */
        add_node_end(&stopped_threads, thread_to_stop);

        /* Remove the thread from the running_threads list */
        delete_node(&running_threads, thread_to_stop);
    }

    /* Try to unlock the kernel mutex */
    if (pthread_mutex_unlock(&kernel_mutex) != 0)
    {
        printf("Cannot unlock the kernel mutex after stopping thread\n");
        return ERROR;
    }

    return !ERROR;  /* Return success */
}

/**
 * @brief Sends a SIGUSR2 signal to resume the execution of a thread.
 * @param thread_to_resume The thread ID of the thread to be resumed.
 * @return Returns `!ERROR` on success, `ERROR` on failure.
 * @details This function sends a SIGUSR2 signal to the specified thread, which triggers the `resume_thread_handler` to resume the thread.
 *          The thread is then moved from the `stopped_threads` list to the `running_threads` list.
 */
int resume_thread(pthread_t thread_to_resume)
{
    /* Variable to store the status of each syscall or library call */
    int status;

    /* Try to lock the kernel mutex */
    if (pthread_mutex_lock(&kernel_mutex) != 0)
    {
        printf("Cannot lock the kernel mutex to resume thread\n");
        return ERROR;
    }

    /* Search for the thread in the running_threads list */
    if (search_node(&running_threads, thread_to_resume) != NULL)
    {
        printf("Thread is already running\n");
        pthread_mutex_unlock(&kernel_mutex);
        return ERROR;
    }

    /* Try to send the resume signal (SIGUSR2) to the thread */
    if (pthread_kill(thread_to_resume, SIGUSR2) != 0)
    {
        printf("Cannot send resume signal\n");
        pthread_mutex_unlock(&kernel_mutex);
        return ERROR;
    }

    /* If the thread is not the main thread, update the lists */
    if (thread_to_resume != main_thread)
    {
        /* Add the resumed thread to the running_threads list */
        add_node_end(&running_threads, thread_to_resume);

        /* Remove the thread from the stopped_threads list */
        delete_node(&stopped_threads, thread_to_resume);
    }

    /* Try to unlock the kernel mutex */
    if (pthread_mutex_unlock(&kernel_mutex) != 0)
    {
        printf("Cannot unlock the kernel mutex after resuming thread\n");
        return ERROR;
    }

    return !ERROR;  /* Return success */
}
/**
 * @brief Entry point function for worker threads.
 * @param arg The argument passed to the thread (unused).
 * @details This function simulates a task by incrementing a dummy variable and then resumes the main thread before exiting.
 */
void pthread_body(void *arg)
{
    sleep(1);  /* Simulate some initial delay */
    volatile int dummy = 0;  /* Dummy variable to simulate work */

    /* Simulate work by incrementing the dummy variable */
    for (int i = 1; i < 100; i++)
    {
        dummy++;
    }

    printf("Thread finished\n");  /* Indicate that the thread has completed its work */
    resume_main();  /* Resume the main thread */
    printf("Main resumed and task exits\n");  /* Indicate that the main thread has been resumed */

    pthread_exit((void *)10);  /* Exit the thread with a return value */
}

/**
 * @brief Initializes signal handlers for SIGUSR1 and SIGUSR2.
 * @details This function sets up the signal handlers for stopping and resuming threads using SIGUSR1 and SIGUSR2, respectively.
 */
void init_signals()
{
    struct sigaction sigusr1, sigusr2;

    /* Configure SIGUSR1 handler to stop threads */
    sigusr1.sa_flags = 0;
    sigusr1.sa_handler = stop_thread_handler;
    sigemptyset(&sigusr1.sa_mask);

    /* Configure SIGUSR2 handler to resume threads */
    sigusr2.sa_flags = 0;
    sigusr2.sa_handler = resume_thread_handler;
    sigusr2.sa_mask = sigusr1.sa_mask;

    /* Register SIGUSR1 handler */
    if (sigaction(SIGUSR1, &sigusr1, NULL) == -1)
    {
        printf("Error in initializing the stop handler\n");
    }

    /* Register SIGUSR2 handler */
    if (sigaction(SIGUSR2, &sigusr2, NULL) == -1)
    {
        printf("Error in initializing the resume handler\n");
    }
}

/**
 * @brief Initializes the threads and their associated data structures.
 * @details This function initializes the `running_threads` and `stopped_threads` lists, sets thread attributes, and creates worker threads.
 */
void init_threads()
{
    /* Initialize the lists for running and stopped threads */
    init_list(&running_threads);
    init_list(&stopped_threads);

    /* Initialize thread attributes */
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* Set thread scheduling parameters */
    struct sched_param param;
    param.sched_priority = 0;  /* Set thread priority */
    int policy = SCHED_FIFO;   /* Set scheduling policy to FIFO */
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setschedpolicy(&attr, &policy);

    /* Create worker threads */
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        if (pthread_create(&threads[i], &attr, pthread_body, (void *)i) != 0)
        {
            printf("Error in creating thread[%d]\n", i);
        }
        else
        {
            /* Add the newly created thread to the running_threads list */
            add_node_end(&running_threads, threads[i]);
        }
    }
}

#ifdef POSIX_TIMER
void increment_tick(int sig)
{
    // tick++;
    return;
}
#endif
/**
 * @brief Stops the main thread by waiting on a condition variable.
 * @details This function locks the `main_mutex` and waits on the `main_cond` condition variable, effectively pausing the main thread.
 *          The main thread will remain blocked until `resume_main` is called.
 */
void stop_main()
{
    pthread_mutex_lock(&main_mutex);  /* Lock the main mutex */
    pthread_cond_wait(&main_cond, &main_mutex);  /* Wait for the condition variable */
    pthread_mutex_unlock(&main_mutex);  /* Unlock the main mutex */
}

/**
 * @brief Resumes the main thread by signaling the condition variable.
 * @details This function signals the `main_cond` condition variable, which wakes up the main thread if it is waiting in `stop_main`.
 */
void resume_main()
{
    pthread_cond_signal(&main_cond);  /* Signal the condition variable to resume the main thread */
}