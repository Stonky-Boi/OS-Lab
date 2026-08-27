#ifndef UTILS_H
#define UTILS_H

void swap(int *a, int *b);

int partition(int a[], int l, int r);
void quick_sort(int a[], int l, int r);

void merge(int a[], int l, int m, int r);

int binary_search(int a[], int n, int x);

void print_array(int a[], int n);

int read_array(int a[], int n);
int read_integer(const char *text, int *x);

#endif