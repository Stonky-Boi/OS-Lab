#ifndef UTILS_H
#define UTILS_H

#include <sys/times.h>
#include <time.h>

int parse_argument(const char *arg, int min_value, int max_value, int *value);
void print_array(int arr[], int n);

void generate_fibonacci(long long seq[], int n);
long long calculate_factorial(int n);

void swap(int *a, int *b);
int partition(int arr[], int l, int r);
void quick_sort(int arr[], int l, int r);
void merge(int arr[], int l, int m, int r);

int start_timing(struct tms *start_times, clock_t *start_real_time);
int end_timing(struct tms *end_times, clock_t *end_real_time);
int print_timing(const struct tms *start_times, const struct tms *end_times, clock_t start_real_time, clock_t end_real_time);

#endif