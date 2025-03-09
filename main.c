#include "rtos/rtos.h"
/* array to store the created threads initially */
extern pthread_t threads[NUMBER_OF_THREADS];
extern pthread_t main_thread;
int main()
{
    // set_callback(increment_tick);
    main_thread = pthread_self();
    printf("m\n");
    pthread_setschedprio(main_thread,3);
    init_signals();
    printf("signals init done\n");
    init_threads();
    printf("signals done\n");
    // osEE_linux_system_timer_init();
    for(int i = 0;i<NUMBER_OF_THREADS&& threads[i]!= -1;i++){
        printf("Thread[%d] stopped\n",i);
        stop_thread(threads[i]);
    }
    printf("\n**************************");
    printf("\nALL THREADS STOPPED\n");
    printf("\n**************************\n");

    for(int i = 0;i<NUMBER_OF_THREADS&& threads[i]!= -1;i++)
    {
        resume_thread(threads[i]);
        printf("Thread[%d] resumed\n",i);
        stop_thread(main_thread);
        stop_thread(threads[i]);
        printf("Thread[%d] stopped\n",i);
    }
    sleep(1);
    return 0;
}