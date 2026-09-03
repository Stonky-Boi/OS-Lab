#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>

#include "utils.h"

int parse_argument(const char *arg, int min_value, int max_value, int *value)
{
    char *end_pointer;
    long parsed_value = strtol(arg, &end_pointer, 10);
    if (end_pointer == arg || *end_pointer != '\0')
        return -1;
    if (parsed_value < min_value || parsed_value > max_value)
        return -1;
    *value = (int)parsed_value;
    return 0;
}

void print_array(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void generate_fibonacci(long long seq[], int n)
{
    if (n >= 1)
        seq[0] = 0;
    if (n >= 2)
        seq[1] = 1;
    for (int i = 2; i < n; i++)
        seq[i] = seq[i - 1] + seq[i - 2];
}

long long calculate_factorial(int n)
{
    long long result = 1;
    for (int i = 2; i <= n; i++)
        result *= i;
    return result;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int l, int r)
{
    int pivot = arr[r];
    int i = l - 1;
    for (int j = l; j < r; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[r]);
    return i + 1;
}

void quick_sort(int arr[], int l, int r)
{
    if (l >= r)
        return;
    int pivot = partition(arr, l, r);
    quick_sort(arr, l, pivot - 1);
    quick_sort(arr, pivot + 1, r);
}

void merge(int arr[], int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    int left[n1];
    int right[n2];
    for (int i = 0; i < n1; i++)
        left[i] = arr[l + i];
    for (int i = 0; i < n2; i++)
        right[i] = arr[m + 1 + i];
    int i = 0;
    int j = 0;
    int k = l;
    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = left[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = right[j];
        j++;
        k++;
    }
}

int start_timing(struct tms *start_times, clock_t *start_real_time)
{
    *start_real_time = times(start_times);
    if (*start_real_time == (clock_t)-1)
    {
        perror("times failed");
        return -1;
    }
    return 0;
}

int end_timing(struct tms *end_times, clock_t *end_real_time)
{
    *end_real_time = times(end_times);
    if (*end_real_time == (clock_t)-1)
    {
        perror("times failed");
        return -1;
    }
    return 0;
}

int print_timing(const struct tms *start_times, const struct tms *end_times, clock_t start_real_time, clock_t end_real_time)
{
    long ticks = sysconf(_SC_CLK_TCK);
    if (ticks == -1)
    {
        perror("sysconf failed");
        return -1;
    }
    double real_time = (double)(end_real_time - start_real_time) / ticks;
    double user_time = (double)(end_times->tms_utime - start_times->tms_utime) / ticks;
    double system_time = (double)(end_times->tms_stime - start_times->tms_stime) / ticks;
    printf("\nInternal timing using times():\n");
    printf("Real time:   %.6f seconds\n", real_time);
    printf("User time:   %.6f seconds\n", user_time);
    printf("System time: %.6f seconds\n", system_time);
    return 0;
}