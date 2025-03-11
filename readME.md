to build
```bash
gcc main.c threads_linked_list/threads_linked_list.c posix_timer/ee_linux_system_timer.c rtos/rtos.c -pthread -o main.out
```

to run
```bash
./main.out > test.txt
```