#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

const int MAX_FIBONACCI_COUNT = 92;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <number_of_fibonacci_numbers>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int count;
    if (parse_argument(argv[1], 1, MAX_FIBONACCI_COUNT, &count) < 0)
    {
        fprintf(stderr, "Enter a number between 1 and %d\n", MAX_FIBONACCI_COUNT);
        return EXIT_FAILURE;
    }
    long long *sequence = malloc((size_t)count * sizeof(long long));
    if (sequence == NULL)
    {
        perror("malloc failed");
        return EXIT_FAILURE;
    }
    struct tms start_times;
    struct tms end_times;
    clock_t start_real_time;
    clock_t end_real_time;
    if (start_timing(&start_times, &start_real_time) < 0)
    {
        free(sequence);
        return EXIT_FAILURE;
    }
    generate_fibonacci(sequence, count);
    if (end_timing(&end_times, &end_real_time) < 0)
    {
        free(sequence);
        return EXIT_FAILURE;
    }
    printf("Fibonacci sequence:\n");
    for (int i = 0; i < count; i++)
        printf("%lld ", sequence[i]);
    printf("\n");
    if (print_timing(&start_times, &end_times, start_real_time, end_real_time) < 0)
    {
        free(sequence);
        return EXIT_FAILURE;
    }
    free(sequence);
    return EXIT_SUCCESS;
}