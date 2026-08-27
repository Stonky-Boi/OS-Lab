#include <stdio.h>
#include "utils.h"

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int a[], int l, int r)
{
    int pivot = a[r];
    int i = l - 1;
    for (int j = l; j < r; j++)
    {
        if (a[j] <= pivot)
        {
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[r]);
    return i + 1;
}

void quick_sort(int a[], int l, int r)
{
    if (l >= r)
        return;
    int i = partition(a, l, r);
    quick_sort(a, l, i - 1);
    quick_sort(a, i + 1, r);
}

void merge(int a[], int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    int left[n1];
    int right[n2];
    for (int i = 0; i < n1; i++)
        left[i] = a[l + i];
    for (int i = 0; i < n2; i++)
        right[i] = a[m + 1 + i];
    int i = 0;
    int j = 0;
    int k = l;
    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
        {
            a[k] = left[i];
            i++;
        }
        else
        {
            a[k] = right[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        a[k] = left[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        a[k] = right[j];
        j++;
        k++;
    }
}

int binary_search(int a[], int n, int x)
{
    int l = 0;
    int r = n - 1;
    while (l <= r)
    {
        int m = l + (r - l) / 2;
        if (a[m] == x)
            return m;
        if (a[m] < x)
            l = m + 1;
        else
            r = m - 1;
    }
    return -1;
}

void print_array(int a[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
}

int read_array(int a[], int n)
{
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", &a[i]) != 1)
        {
            fprintf(stderr, "Invalid input\n");
            return -1;
        }
    }
    return 0;
}

int read_integer(const char *text, int *x)
{
    printf("%s", text);
    if (scanf("%d", x) != 1)
    {
        fprintf(stderr, "Invalid input\n");
        return -1;
    }
    return 0;
}