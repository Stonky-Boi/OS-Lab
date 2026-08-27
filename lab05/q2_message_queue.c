#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"

#define ARRAY_SIZE 10

struct search_message
{
    long type;
    int sorted_array[ARRAY_SIZE];
    int target;
};

struct result_message
{
    long type;
    int result;
};

int main()
{
    int array[ARRAY_SIZE];
    int target;
    if (read_array(array, ARRAY_SIZE) < 0)
        return EXIT_FAILURE;
    if (read_integer("Enter element to search: ", &target) < 0)
        return EXIT_FAILURE;
    quick_sort(array, 0, ARRAY_SIZE - 1);
    printf("Sorted array:\n");
    print_array(array, ARRAY_SIZE);
    int message_queue_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (message_queue_id < 0)
    {
        perror("msgget failed");
        return EXIT_FAILURE;
    }
    pid_t child_process = fork();
    if (child_process < 0)
    {
        perror("fork failed");
        msgctl(message_queue_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    if (child_process == 0)
    {
        struct search_message search_message;
        if (msgrcv(message_queue_id, &search_message, sizeof(search_message) - sizeof(long), 1, 0) < 0)
        {
            perror("msgrcv failed");
            return EXIT_FAILURE;
        }
        int result = binary_search(search_message.sorted_array, ARRAY_SIZE, search_message.target);
        struct result_message result_message;
        result_message.type = 2;
        result_message.result = result;
        if (msgsnd(message_queue_id, &result_message, sizeof(result_message.result), 0) < 0)
        {
            perror("msgsnd failed");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    struct search_message search_message;
    search_message.type = 1;
    search_message.target = target;
    for (int i = 0; i < ARRAY_SIZE; i++)
        search_message.sorted_array[i] = array[i];
    if (msgsnd(message_queue_id, &search_message, sizeof(search_message) - sizeof(long), 0) < 0)
    {
        perror("msgsnd failed");
        waitpid(child_process, NULL, 0);
        msgctl(message_queue_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    struct result_message result_message;
    if (msgrcv(message_queue_id, &result_message, sizeof(result_message.result), 2, 0) < 0)
    {
        perror("msgrcv failed");
        waitpid(child_process, NULL, 0);
        msgctl(message_queue_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    waitpid(child_process, NULL, 0);
    printf("Index returned by child: %d\n", result_message.result);
    msgctl(message_queue_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}