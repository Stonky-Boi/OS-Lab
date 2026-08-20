#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int shared_variable = 100;

static void print_memory_state(const char *process_name, int *local_variable)
{
    printf("%s Process ID: %d\n", process_name, getpid());
    printf("Shared Variable:\n");
    printf("\tAddress: %p\n", (void *)&shared_variable);
    printf("\tValue: %d\n", shared_variable);
    printf("Local Variable:\n");
    printf("\tAddress: %p\n", (void *)local_variable);
    printf("\tValue: %d\n\n", *local_variable);
}

int main()
{
    int local_variable = 200;
    print_memory_state("Initial", &local_variable);
    pid_t process_id = fork();
    if (process_id < 0)
    {
        perror("Fork Failed");
        return EXIT_FAILURE;
    }
    if (process_id == 0)
    {
        shared_variable += 50;
        local_variable += 50;
        print_memory_state("Child", &local_variable);
        return EXIT_SUCCESS;
    }
    shared_variable += 100;
    local_variable += 100;
    print_memory_state("Parent", &local_variable);
    wait(NULL);
    return EXIT_SUCCESS;
}