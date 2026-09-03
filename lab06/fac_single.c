#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

const int MAX_FACTORIAL = 20;

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
    int number = parsed_number;
    struct tms start_times;
    struct tms end_times;
    clock_t start_real_time;
    clock_t end_real_time;
    if (start_timing(&start_times, &start_real_time) < 0)
        return EXIT_FAILURE;
    long long factorial_result = calculate_factorial(number);
    if (end_timing(&end_times, &end_real_time) < 0)
        return EXIT_FAILURE;
    printf("Factorial of %u: %lld\n", number, factorial_result);
    if (print_timing(&start_times, &end_times, start_real_time, end_real_time) < 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}