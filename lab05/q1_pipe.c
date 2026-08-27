#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"

#define ARRAY_SIZE 10
#define HALF_SIZE 5

int main()
{
    int input_array[ARRAY_SIZE];
    int p_c1[2];
    int c1_p[2];
    int p_c2[2];
    int c2_p[2];
    if (pipe(p_c1) < 0 || pipe(c1_p) < 0 || pipe(p_c2) < 0 || pipe(c2_p) < 0)
    {
        perror("pipe failed");
        return EXIT_FAILURE;
    }
    if (read_array(input_array, ARRAY_SIZE) < 0)
        return EXIT_FAILURE;
    pid_t c1 = fork();
    if (c1 < 0)
    {
        perror("fork failed");
        return EXIT_FAILURE;
    }
    if (c1 == 0)
    {
        int array[HALF_SIZE];
        close(p_c1[1]);
        close(c1_p[0]);
        ssize_t bytes_read = read(p_c1[0], array, sizeof(array));
        if (bytes_read != sizeof(array))
        {
            perror("read failed");
            return EXIT_FAILURE;
        }
        quick_sort(array, 0, HALF_SIZE - 1);
        ssize_t bytes_written = write(c1_p[1], array, sizeof(array));
        if (bytes_written != sizeof(array))
        {
            perror("write failed");
            return EXIT_FAILURE;
        }
        close(p_c1[0]);
        close(c1_p[1]);
        return EXIT_SUCCESS;
    }
    pid_t c2 = fork();
    if (c2 < 0)
    {
        perror("fork failed");
        waitpid(c1, NULL, 0);
        return EXIT_FAILURE;
    }
    if (c2 == 0)
    {
        int array[HALF_SIZE];
        close(p_c2[1]);
        close(c2_p[0]);
        ssize_t bytes_read = read(p_c2[0], array, sizeof(array));
        if (bytes_read != sizeof(array))
        {
            perror("read failed");
            return EXIT_FAILURE;
        }
        quick_sort(array, 0, HALF_SIZE - 1);
        ssize_t bytes_written = write(c2_p[1], array, sizeof(array));
        if (bytes_written != sizeof(array))
        {
            perror("write failed");
            return EXIT_FAILURE;
        }
        close(p_c2[0]);
        close(c2_p[1]);
        return EXIT_SUCCESS;
    }
    close(p_c1[0]);
    close(c1_p[1]);
    close(p_c2[0]);
    close(c2_p[1]);
    write(p_c1[1], input_array, HALF_SIZE * sizeof(int));
    write(p_c2[1], input_array + HALF_SIZE, HALF_SIZE * sizeof(int));
    close(p_c1[1]);
    close(p_c2[1]);
    int first_half[HALF_SIZE];
    int second_half[HALF_SIZE];
    read(c1_p[0], first_half, sizeof(first_half));
    read(c2_p[0], second_half, sizeof(second_half));
    close(c1_p[0]);
    close(c2_p[0]);
    waitpid(c1, NULL, 0);
    waitpid(c2, NULL, 0);
    int sorted_array[ARRAY_SIZE];
    for (int i = 0; i < HALF_SIZE; i++)
    {
        sorted_array[i] = first_half[i];
        sorted_array[i + HALF_SIZE] = second_half[i];
    }
    merge(sorted_array, 0, HALF_SIZE - 1, ARRAY_SIZE - 1);
    printf("Sorted array:\n");
    print_array(sorted_array, ARRAY_SIZE);
    return EXIT_SUCCESS;
}