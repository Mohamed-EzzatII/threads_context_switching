/**
 * @file main.c
 * @brief Main file for demonstrating pthread management.
 * @author Mohamed Ezzat
 * @date 2025-03-11
 */

 #include "pthreads_switching/pthreads_switching.h"

 /* Array to store the created threads initially */
 extern pthread_t threads[NUMBER_OF_THREADS];
 
 /* Variable to store the main thread ID */
 extern pthread_t main_thread;
 
 /**
  * @brief Main function to demonstrate thread management.
  * @details This function initializes signals, creates worker threads, and demonstrates stopping and resuming threads.
  * @return Returns 0 on successful execution.
  */
 int main()
 {
     printf("Demo is running\n");
 
 #ifdef POSIX_TIMER
     /* Set the callback function for the timer (if POSIX_TIMER is defined) */
     set_callback(increment_tick);
 #endif
 
     /* Store the main thread ID and set its priority */
     main_thread = pthread_self();
     pthread_setschedprio(main_thread, 3);
 
     /* Initialize signal handlers */
     init_signals();
     printf("Signals init done\n");
 
     /* Initialize worker threads */
     init_threads();
     printf("Threads init done\n");
 
 #ifdef POSIX_TIMER
     /* Initialize the system timer (if POSIX_TIMER is defined) */
     osEE_linux_system_timer_init();
 #endif
 
     /* Stop all threads */
     for (int i = 0; i < NUMBER_OF_THREADS && threads[i] != -1; i++)
     {
         printf("Thread[%d] stopped\n", i);
         stop_thread(threads[i]);
     }
 
     printf("\n**************************");
     printf("\nALL THREADS STOPPED\n");
     printf("\n**************************\n");
 
     /* Resume and stop threads one by one */
     for (int i = 0; i < NUMBER_OF_THREADS && threads[i] != -1; i++)
     {
         resume_thread(threads[i]);
         printf("Thread[%d] resumed\n", i);
 
         stop_main();  /* Stop the main thread */
         printf("Main thread works\n");
 
         stop_thread(threads[i]);
         printf("Thread[%d] stopped\n", i);
     }
 
     printf("\n**************************");
     printf("\nDEMO FINISHED\n");
     printf("\n**************************\n");
 
     return 0;  /* Return success */
 }