/**
 * @file pthreads_switching.h
 * @brief Header file for a simple switching between pthreads
 * @author Mohamed Ezzat
 * @date 2025-3-11
 */

 #ifndef __PTHREAD_SWITCHING__
 #define __PTHREAD_SWITCHING__
 
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
 
 #define NUMBER_OF_THREADS 20  /**< Maximum number of threads supported by the system. */
 #define ERROR 0               /**< Error return value. */
 
 /**
  * @brief Flag to indicate that a signal is unhandled.
  */
 #define SIGNAL_UNHANDLED 0
 
 /**
  * @brief Flag to indicate that a signal is handled.
  */
 #define SIGNAL_HANDLED 1
 
 /**
  * @brief Sends a SIGUSR1 signal to stop the execution of a thread.
  * @param thread_to_stop The thread ID of the thread to be stopped.
  * @return Returns `!ERROR` on success, `ERROR` on failure.
  */
 int stop_thread(pthread_t thread_to_stop);
 
 /**
  * @brief Sends a SIGUSR2 signal to resume the execution of a thread.
  * @param thread_to_resume The thread ID of the thread to be resumed.
  * @return Returns `!ERROR` on success, `ERROR` on failure.
  */
 int resume_thread(pthread_t thread_to_resume);
 
 /**
  * @brief Initializes signal handlers for SIGUSR1 and SIGUSR2.
  */
 void init_signals();
 
 /**
  * @brief Initializes the worker threads and their associated data structures.
  */
 void init_threads();
 
 /**
  * @brief Stops the main thread by waiting on a condition variable.
  */
 void stop_main();
 
 /**
  * @brief Resumes the main thread by signaling the condition variable.
  */
 void resume_main();
 
 #endif /* __PTHREAD_SWITCHING__ */