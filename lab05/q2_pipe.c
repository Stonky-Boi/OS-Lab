#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"

#define ARRAY_SIZE 10

int main()
{
    int array[ARRAY_SIZE];
    int target;
    int p_c[2];
    int c_p[2];
    if (pipe(p_c) < 0 || pipe(c_p) < 0)
    {
        perror("pipe failed");
        return EXIT_FAILURE;
    }
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
        int received_array[ARRAY_SIZE];
        int received_target;
        close(p_c[1]);
        close(c_p[0]);
        read(p_c[0], received_array, sizeof(received_array));
        read(p_c[0], &received_target, sizeof(received_target));
        close(p_c[0]);
        int result = binary_search(received_array, ARRAY_SIZE, received_target);
        write(c_p[1], &result, sizeof(result));
        close(c_p[1]);
        return EXIT_SUCCESS;
    }
    close(p_c[0]);
    close(c_p[1]);
    write(p_c[1], array, sizeof(array));
    write(p_c[1], &target, sizeof(target));
    close(p_c[1]);
    int result;
    read(c_p[0], &result, sizeof(result));
    close(c_p[0]);
    waitpid(child_process, NULL, 0);
    printf("Index returned by child: %d\n", result);
    return EXIT_SUCCESS;
}