#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"

#define ARRAY_SIZE 10
#define HALF_SIZE 5
#define SHARED_MEMORY_PERMISSIONS 0666

int main()
{
    int shared_memory_id;
    int *shared_array;
    shared_memory_id = shmget(IPC_PRIVATE, ARRAY_SIZE * sizeof(int), IPC_CREAT | SHARED_MEMORY_PERMISSIONS);
    if (shared_memory_id < 0)
    {
        perror("shmget failed");
        return EXIT_FAILURE;
    }
    shared_array = shmat(shared_memory_id, NULL, 0);
    if (shared_array == (void *)-1)
    {
        perror("shmat failed");
        shmctl(shared_memory_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    if (read_array(shared_array, ARRAY_SIZE) < 0)
    {
        shmdt(shared_array);
        shmctl(shared_memory_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    pid_t c1 = fork();
    if (c1 < 0)
    {
        perror("fork failed");
        shmdt(shared_array);
        shmctl(shared_memory_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    if (c1 == 0)
    {
        quick_sort(shared_array, 0, HALF_SIZE - 1);
        shmdt(shared_array);
        return EXIT_SUCCESS;
    }
    pid_t c2 = fork();
    if (c2 < 0)
    {
        perror("fork failed");
        waitpid(c1, NULL, 0);
        shmdt(shared_array);
        shmctl(shared_memory_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    if (c2 == 0)
    {
        quick_sort(shared_array, HALF_SIZE, ARRAY_SIZE - 1);
        shmdt(shared_array);
        return EXIT_SUCCESS;
    }
    waitpid(c1, NULL, 0);
    waitpid(c2, NULL, 0);
    merge(shared_array, 0, HALF_SIZE - 1, ARRAY_SIZE - 1);
    printf("Sorted array:\n");
    print_array(shared_array, ARRAY_SIZE);
    shmdt(shared_array);
    shmctl(shared_memory_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}