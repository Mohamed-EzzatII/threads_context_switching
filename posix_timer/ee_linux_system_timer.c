/* ###*B*###
 * --------------------------------------------------------------------------
 * |              _    _ _______     .----.      _____         _____        |
 * |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
 * |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
 * |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
 * |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
 * |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
 * |                              . _ _  .                                  |
 * --------------------------------------------------------------------------
 * 
 * All Rights Reserved.
 * ny use of this source code is subject to a license agreement with the
 * AUTOSAR development cooperation.
 * More information is available at www.autosar.org.
 * 
 * Disclaimer
 * 
 * This work (specification and/or software implementation) and the material
 * contained in it, as released by AUTOSAR, is for the purpose of information
 * only. AUTOSAR and the companies that have contributed to it shall not be
 * liable for any use of the work.
 * 
 * The material contained in this work is protected by copyright and other
 * types of intellectual property rights. The commercial exploitation of the
 * material contained in this work requires a license to such intellectual
 * property rights.
 * 
 * This work may be utilized or reproduced without any modification, in any
 * form or by any means, for informational purposes only. For any other
 * purpose, no part of the work may be utilized or reproduced, in any form
 * or by any means, without permission in writing from the publisher.
 * 
 * The work has been developed for automotive applications only. It has
 * neither been developed, nor tested for non-automotive applications.
 * 
 * The word AUTOSAR and the AUTOSAR logo are registered trademarks.
 * --------------------------------------------------------------------------
 * ###*E*### */

/** \file	ee_linux_system_timer.c
 *  \brief	System Timer driver source file.
 *
 *  \note	TO BE DOCUMENTED!!!
 *
 *  \author	Mohamed Ezzat
 *  \date	2025
 */

/* ERIKA Enterprise Internals */
// #include "ee_internal.h"

/* Include necessary headers */
#include <signal.h>   /* For signal handling functions (e.g., sigaction) */
#include <time.h>     /* For timer functions (e.g., timer_create, timer_settime) */
#include <stdio.h>    /* For standard I/O functions (e.g., printf) */
#include <stdlib.h>   /* For standard library functions (e.g., exit) */
#include <stdint.h>   /* For fixed-width integer types (not used in this example) */
#include <unistd.h>   /* For POSIX API functions (e.g., pause) */
#include <pthread.h>  /* For thread-related functions (e.g., pthread_self) */

#define OSTICKDURATION 1
#define OSEE_KILO 1000
#define OSEE_MEGA  1000000
// #if	(defined(OSEE_SYSTEM_TIMER_DEVICE))
void (*callback)();
/** \brief Internal variable to store the created timer ID */
static timer_t timerid; 

/** \brief Internal variable to store the timer settings */
static struct itimerspec curr_value; 
void signal_handler(int sig) {

  callback();
}
/** \brief	System Timer Initialization. */
void osEE_linux_system_timer_init(void)
{ 
    /* Step 1: Set up the signal handler */
    struct sigaction act;  /* Structure to define the signal handling behavior */
    act.sa_handler = signal_handler;  /* Set the handler function */
    act.sa_flags = 0;  /* No special flags */
    sigemptyset(&act.sa_mask);  /* Initialize the signal mask (no signals are blocked during handler execution) */

    /* Step 2: Create a timer */
    struct sigevent sevp;  /* Structure to define how the timer notifies the process */

    /* Configure the sigevent structure */
    sevp.sigev_signo = SIGALRM;  /* Specify the signal to be sent (SIGALRM) */
    sevp._sigev_un._tid = pthread_self();  /* Set the thread ID (not typically used for timers) */
    sevp.sigev_notify = SIGEV_SIGNAL;  /* Notify using a signal */

    /* Step 3: Set up the timer interval and initial expiration time */
    struct itimerspec value;
    value.it_interval.tv_sec = OSTICKDURATION / OSEE_KILO;  /* Interval for periodic timer in seconds */
    value.it_interval.tv_nsec = (OSTICKDURATION % OSEE_KILO)*OSEE_MEGA; /* Interval for periodic timer */
    value.it_value.tv_sec = OSTICKDURATION / OSEE_KILO; /* Initial expiration time */
    value.it_value.tv_nsec = (OSTICKDURATION % OSEE_KILO)*OSEE_MEGA; /* Initial expiration time (1 nanosecond) */

    /* Step 4: Create the timer */
    if (timer_create(CLOCK_PROCESS_CPUTIME_ID, &sevp, &timerid) == -1) {
        /* If timer creation fails, print an error and exit */
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    /* Step 5: Register the signal handler for SIGALRM */
    if (sigaction(SIGALRM, &act, NULL) == -1) {
        /* If sigaction fails, print an error and exit */
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Step 6: Start the timer */
    if (timer_settime(timerid, 0, &value, NULL) == -1) {
      /* If timer_settime fails, print an error and exit */
      perror("timer_settime");
      exit(EXIT_FAILURE);
    }
}

/** \brief	System Timer Sleep. */
void osEE_linux_system_timer_sleep(void)
{
  if(timer_gettime(timerid, &curr_value) == -1){
    perror("timer_delete");
    exit(EXIT_FAILURE);
  }
  struct itimerspec value;
  value.it_interval.tv_sec = 0;  /* Interval for periodic timer in seconds */
  value.it_interval.tv_nsec = 0; /* Interval for periodic timer nanoseconds */
  value.it_value.tv_sec = 0;     /* Initial expiration time (0 seconds) */
  value.it_value.tv_nsec =0;   /* Initial expiration time (1 nanosecond) */
  if(timer_settime(timerid, 0, &value, NULL) == -1){
    perror("timer_settime");
    exit(EXIT_FAILURE);
  }
}

/** \brief System Timer Wakeup.It will resume from the time at which the timer is stopped with the same settings */
void osEE_linux_system_timer_wakeup(void)
{
  if(timer_settime(timerid, 0, &curr_value, NULL) == -1){
    perror("timer_settime");
    exit(EXIT_FAILURE);
  }
}

void set_callback(void *callback_fn){
callback = callback_fn;

}

/** \brief Signal handler function */


// #endif /* OSEE_SYSTEM_TIMER_DEVICE == OSEE_LINUX_SYSTICK */