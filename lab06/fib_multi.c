#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "utils.h"

const int MAX_FIBONACCI_COUNT = 92;

struct fibonacci_data
{
    long long *sequence;
    int count;
};

static void *generate_thread(void *argument)
{
    struct fibonacci_data *data = argument;
    generate_fibonacci(data->sequence, data->count);
    return NULL;
}

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
    struct fibonacci_data data = {sequence, count};
    struct tms start_times;
    struct tms end_times;
    clock_t start_real_time;
    clock_t end_real_time;
    if (start_timing(&start_times, &start_real_time) < 0)
    {
        free(sequence);
        return EXIT_FAILURE;
    }
    pthread_t thread;
    int result = pthread_create(&thread, NULL, generate_thread, &data);
    if (result != 0)
    {
        fprintf(stderr, "pthread_create failed: %d\n", result);
        free(sequence);
        return EXIT_FAILURE;
    }
    result = pthread_join(thread, NULL);
    if (result != 0)
    {
        fprintf(stderr, "pthread_join failed: %d\n", result);
        free(sequence);
        return EXIT_FAILURE;
    }
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