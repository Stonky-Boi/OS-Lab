#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"

#define ARRAY_SIZE 10

extern char **environ;

static int run_binary_search(int argc, char *args[])
{
    if (argc != ARRAY_SIZE + 3)
    {
        fprintf(stderr, "Invalid arguments for binary search\n");
        return EXIT_FAILURE;
    }
    int sorted_array[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++)
        sorted_array[i] = atoi(args[i + 2]);
    int target = atoi(args[ARRAY_SIZE + 2]);
    int result = binary_search(sorted_array, ARRAY_SIZE, target);
    printf("Binary search result: %d\n", result);
    if (result == -1)
        return 255;
    return result;
}

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--binary-search") == 0)
        return run_binary_search(argc, argv);
    int array[ARRAY_SIZE];
    int target;
    if (read_array(array, ARRAY_SIZE) < 0)
        return EXIT_FAILURE;
    if (read_integer("Enter element to search: ", &target) < 0)
        return EXIT_FAILURE;
    quick_sort(array, 0, ARRAY_SIZE - 1);
    printf("Sorted array:\n");
    print_array(array, ARRAY_SIZE);
    pid_t child_process = fork();
    if (child_process < 0)
    {
        perror("fork failed");
        return EXIT_FAILURE;
    }
    if (child_process == 0)
    {
        char num_args[ARRAY_SIZE][20];
        char target_arg[20];
        char *exec_args[ARRAY_SIZE + 4];
        exec_args[0] = argv[0];
        exec_args[1] = "--binary-search";
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            snprintf(num_args[i], sizeof(num_args[i]), "%d", array[i]);
            exec_args[i + 2] = num_args[i];
        }
        snprintf(target_arg, sizeof(target_arg), "%d", target);
        exec_args[ARRAY_SIZE + 2] = target_arg;
        exec_args[ARRAY_SIZE + 3] = NULL;
        execve(argv[0], exec_args, environ);
        perror("execve failed");
        return EXIT_FAILURE;
    }
    int child_status;
    if (waitpid(child_process, &child_status, 0) < 0)
    {
        perror("waitpid failed");
        return EXIT_FAILURE;
    }
    if (!WIFEXITED(child_status))
    {
        fprintf(stderr, "Child did not exit normally\n");
        return EXIT_FAILURE;
    }
    int result = WEXITSTATUS(child_status);
    if (result == 255)
        result = -1;
    printf("Index returned by child: %d\n", result);
    return EXIT_SUCCESS;
}