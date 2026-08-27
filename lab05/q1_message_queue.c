#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"

#define ARRAY_SIZE 10
#define HALF_SIZE 5

struct sort_message
{
    long type;
    int values[HALF_SIZE];
};

int main()
{
    int input_array[ARRAY_SIZE];
    if (read_array(input_array, ARRAY_SIZE) < 0)
        return EXIT_FAILURE;
    int message_queue_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (message_queue_id < 0)
    {
        perror("msgget failed");
        return EXIT_FAILURE;
    }
    pid_t c1 = fork();
    if (c1 < 0)
    {
        perror("fork failed");
        msgctl(message_queue_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    if (c1 == 0)
    {
        struct sort_message message;
        if (msgrcv(message_queue_id, &message, sizeof(message.values), 1, 0) < 0)
        {
            perror("msgrcv failed");
            return EXIT_FAILURE;
        }
        quick_sort(message.values, 0, HALF_SIZE - 1);
        message.type = 3;
        if (msgsnd(message_queue_id, &message, sizeof(message.values), 0) < 0)
        {
            perror("msgsnd failed");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    pid_t c2 = fork();
    if (c2 < 0)
    {
        perror("fork failed");
        waitpid(c1, NULL, 0);
        msgctl(message_queue_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    if (c2 == 0)
    {
        struct sort_message message;
        if (msgrcv(message_queue_id, &message, sizeof(message.values), 2, 0) < 0)
        {
            perror("msgrcv failed");
            return EXIT_FAILURE;
        }
        quick_sort(message.values, 0, HALF_SIZE - 1);
        message.type = 4;
        if (msgsnd(message_queue_id, &message, sizeof(message.values), 0) < 0)
        {
            perror("msgsnd failed");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    struct sort_message m1;
    m1.type = 1;
    struct sort_message m2;
    m2.type = 2;
    for (int i = 0; i < HALF_SIZE; i++)
    {
        m1.values[i] = input_array[i];
        m2.values[i] = input_array[i + HALF_SIZE];
    }
    if (msgsnd(message_queue_id, &m1, sizeof(m1.values), 0) < 0)
    {
        perror("msgsnd failed");
        return EXIT_FAILURE;
    }
    if (msgsnd(message_queue_id, &m2, sizeof(m2.values), 0) < 0)
    {
        perror("msgsnd failed");
        return EXIT_FAILURE;
    }
    struct sort_message sorted1;
    struct sort_message sorted2;
    if (msgrcv(message_queue_id, &sorted1, sizeof(sorted1.values), 3, 0) < 0)
    {
        perror("msgrcv failed");
        return EXIT_FAILURE;
    }
    if (msgrcv(message_queue_id, &sorted2, sizeof(sorted2.values), 4, 0) < 0)
    {
        perror("msgrcv failed");
        return EXIT_FAILURE;
    }
    waitpid(c1, NULL, 0);
    waitpid(c2, NULL, 0);
    int sorted_array[ARRAY_SIZE];
    for (int i = 0; i < HALF_SIZE; i++)
    {
        sorted_array[i] = sorted1.values[i];
        sorted_array[i + HALF_SIZE] = sorted2.values[i];
    }
    merge(sorted_array, 0, HALF_SIZE - 1, ARRAY_SIZE - 1);
    printf("Sorted array:\n");
    print_array(sorted_array, ARRAY_SIZE);
    msgctl(message_queue_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}