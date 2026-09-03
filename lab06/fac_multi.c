#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "utils.h"

const int MAX_FACTORIAL = 20;

struct factorial_data
{
    int number;
    long long result;
};

static void *calculate_thread(void *argument)
{
    struct factorial_data *data = argument;
    data->result = calculate_factorial(data->number);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int parsed_number;
    if (parse_argument(argv[1], 0, MAX_FACTORIAL, &parsed_number) < 0)
    {
        fprintf(stderr, "Enter an integer between 0 and %d\n", MAX_FACTORIAL);
        return EXIT_FAILURE;
    }
    struct factorial_data data = {parsed_number, 0};
    struct tms start_times;
    struct tms end_times;
    clock_t start_real_time;
    clock_t end_real_time;
    if (start_timing(&start_times, &start_real_time) < 0)
        return EXIT_FAILURE;
    pthread_t thread;
    int result = pthread_create(&thread, NULL, calculate_thread, &data);
    if (result != 0)
    {
        fprintf(stderr, "pthread_create failed: %d\n", result);
        return EXIT_FAILURE;
    }
    result = pthread_join(thread, NULL);
    if (result != 0)
    {
        fprintf(stderr, "pthread_join failed: %d\n", result);
        return EXIT_FAILURE;
    }
    if (end_timing(&end_times, &end_real_time) < 0)
        return EXIT_FAILURE;
    printf("Factorial of %u: %lld\n", data.number, data.result);
    if (print_timing(&start_times, &end_times, start_real_time, end_real_time) < 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}