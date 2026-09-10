#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void *thread_function(void *argument)
{
    (void)argument;
    printf("Thread is running.\n");
    return NULL;
}

static void print_scope_name(int scope)
{
    if (scope == PTHREAD_SCOPE_SYSTEM)
        printf("PTHREAD_SCOPE_SYSTEM\n");
    else if (scope == PTHREAD_SCOPE_PROCESS)
        printf("PTHREAD_SCOPE_PROCESS\n");
    else
        printf("Unknown contention scope\n");
}

int main()
{
    pthread_attr_t thread_attributes;
    int result = pthread_attr_init(&thread_attributes);
    if (result != 0)
    {
        fprintf(stderr, "pthread_attr_init failed: %s\n", strerror(result));
        return EXIT_FAILURE;
    }
    printf("Contention Scope Demonstration\n");
    printf("==============================\n\n");
    int current_scope;
    result = pthread_attr_getscope(&thread_attributes, &current_scope);
    if (result != 0)
    {
        fprintf(stderr, "pthread_attr_getscope failed: %s\n", strerror(result));
        pthread_attr_destroy(&thread_attributes);
        return EXIT_FAILURE;
    }
    printf("Initial contention scope: ");
    print_scope_name(current_scope);
    printf("\nSetting PTHREAD_SCOPE_SYSTEM...\n");
    result = pthread_attr_setscope(&thread_attributes, PTHREAD_SCOPE_SYSTEM);
    if (result != 0)
    {
        fprintf(stderr, "Failed to set PTHREAD_SCOPE_SYSTEM: %s\n", strerror(result));
        pthread_attr_destroy(&thread_attributes);
        return EXIT_FAILURE;
    }
    result = pthread_attr_getscope(&thread_attributes, &current_scope);
    if (result != 0)
    {
        fprintf(stderr, "pthread_attr_getscope failed: %s\n", strerror(result));
        pthread_attr_destroy(&thread_attributes);
        return EXIT_FAILURE;
    }
    printf("Current contention scope: ");
    print_scope_name(current_scope);
    pthread_t thread_id;
    result = pthread_create(&thread_id, &thread_attributes, thread_function, NULL);
    if (result != 0)
    {
        fprintf(stderr, "pthread_create failed: %s\n", strerror(result));
        pthread_attr_destroy(&thread_attributes);
        return EXIT_FAILURE;
    }
    result = pthread_join(thread_id, NULL);
    if (result != 0)
    {
        fprintf(stderr, "pthread_join failed: %s\n", strerror(result));
        pthread_attr_destroy(&thread_attributes);
        return EXIT_FAILURE;
    }
    printf("\nAttempting to set PTHREAD_SCOPE_PROCESS...\n");
    result = pthread_attr_setscope(&thread_attributes, PTHREAD_SCOPE_PROCESS);
    if (result == ENOTSUP)
        printf("PTHREAD_SCOPE_PROCESS is not supported on Linux.\n");
    else if (result != 0)
        fprintf(stderr, "Failed to set PTHREAD_SCOPE_PROCESS: %s\n", strerror(result));
    else
    {
        result = pthread_attr_getscope(&thread_attributes, &current_scope);
        if (result != 0)
        {
            fprintf(stderr, "pthread_attr_getscope failed: %s\n", strerror(result));
            pthread_attr_destroy(&thread_attributes);
            return EXIT_FAILURE;
        }
        printf("Current contention scope: ");
        print_scope_name(current_scope);
    }
    pthread_attr_destroy(&thread_attributes);
    return EXIT_SUCCESS;
}