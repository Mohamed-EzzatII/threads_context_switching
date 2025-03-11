# Thread Management System with Pthreads

This repository contains a simple thread management system implemented using POSIX threads (`pthreads`). The system allows you to create, stop, and resume threads dynamically, demonstrating basic context switching and thread synchronization.

## Repository Structure

The repository is organized as follows:

```
├── main.c
├── pthreads_switching
│   ├── pthreads_switching.c
│   └── pthreads_switching.h
├── readME.md
└── threads_linked_list
    ├── threads_linked_list.c
    └── threads_linked_list.h
```

### Key Files
- **`main.c`**: The entry point of the program. It initializes the system, creates threads, and demonstrates stopping and resuming threads.
- **`pthreads_switching/`**: Contains the implementation of thread management functions, including stopping and resuming threads.
- **`threads_linked_list/`**: Implements a linked list to manage thread IDs for running and stopped threads.

---

## How to Build and Run

### On Linux

#### **Build the Program**:
   ```bash
   gcc main.c threads_linked_list/threads_linked_list.c pthreads_switching/pthreads_switching.c -pthread -o main.out
   ```

#### **Run the Program**:
   ```bash
   ./main.out
   ```

### On Windows

#### **Build the Program**:
   ```bash
   gcc main.c threads_linked_list/threads_linked_list.c pthreads_switching/pthreads_switching.c -o main.exe -lpthread
   ```

#### **Run the Program**:
   ```bash
   main.exe
   ```

---

## Features

- **Thread Creation**: Creates a specified number of worker threads.
- **Thread Stopping**: Stops threads using signals (`SIGUSR1`).
- **Thread Resuming**: Resumes stopped threads using signals (`SIGUSR2`).
- **Linked List Management**: Uses a linked list to keep track of running and stopped threads.
- **Main Thread Synchronization**: Demonstrates stopping and resuming the main thread.



