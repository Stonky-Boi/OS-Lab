#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "utils.h"

const int ARRAY_SIZE = 10;
const int HALF_SIZE = 5;

struct sort_data
{
    int *arr;
    int l;
    int r;
};

static void *sort_partition(void *argument)
{
    struct sort_data *data = argument;
    quick_sort(data->arr, data->l, data->r);
    return NULL;
}

int main()
{
    int arr[ARRAY_SIZE];
    printf("Enter 10 integers:\n");
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (scanf("%d", &arr[i]) != 1)
        {
            fprintf(stderr, "Invalid input\n");
            return EXIT_FAILURE;
        }
    }
    struct sort_data arr1 = {arr, 0, HALF_SIZE - 1};
    struct sort_data arr2 = {arr, HALF_SIZE, ARRAY_SIZE - 1};
    pthread_t thread1;
    pthread_t thread2;
    int result = pthread_create(&thread1, NULL, sort_partition, &arr1);
    if (result != 0)
    {
        fprintf(stderr, "pthread_create for T1 failed: %d\n", result);
        return EXIT_FAILURE;
    }
    result = pthread_create(&thread2, NULL, sort_partition, &arr2);
    if (result != 0)
    {
        fprintf(stderr, "pthread_create for T2 failed: %d\n", result);
        pthread_join(thread1, NULL);
        return EXIT_FAILURE;
    }
    result = pthread_join(thread1, NULL);
    if (result != 0)
    {
        fprintf(stderr, "pthread_join for T1 failed: %d\n", result);
        pthread_join(thread2, NULL);
        return EXIT_FAILURE;
    }
    result = pthread_join(thread2, NULL);
    if (result != 0)
    {
        fprintf(stderr, "pthread_join for T2 failed: %d\n", result);
        return EXIT_FAILURE;
    }
    merge(arr, 0, HALF_SIZE - 1, ARRAY_SIZE - 1);
    printf("Sorted arr:\n");
    print_array(arr, ARRAY_SIZE);
    return EXIT_SUCCESS;
}