#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../threads_linked_list/threads_linked_list.h"
#ifdef POSIX_TIMER
#include "../posix_timer/ee_linux_system_timer.h"
#endif
#ifndef __RTOS__
#define __RTOS__
#define NUMBER_OF_THREADS 20
#define ERROR 0
/*
 * Flag to check if the signal is handled or not
 */
#define SIGNAL_UNHANDLED 0
#define SIGNAL_HANDLED 1
/*
 * send signal SIGUSR1 to stop the thread execution
 */
int stop_thread(pthread_t thread_to_stop);

/*
 * send signal SIGUSR2 to resume the thread execution
 */
int resume_thread(pthread_t thread_to_resume);

/* Initialize the signals */
void init_signals();

/* Initialize the worker threads */
void init_threads();
/* stop main thread */
void stop_main();
/* resume main thread */
void resume_main();

#endif