#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

static void display_cli(void)
{
    printf("========================================\n");
    printf("             MyOS Command CLI\n");
    printf("========================================\n");
    printf("1. Nano Text Editor\n");
    printf("2. Vim Text Editor\n");
    printf("3. Process Monitor (top)\n");
    printf("4. Calculator (bc)\n");
    printf("5. File Pager (less)\n");
    printf("0. Exit CLI\n");
    printf("========================================\n");
}

static void demonstrate_orphan_process(void)
{
    pid_t child_process = fork();
    if (child_process < 0)
    {
        perror("fork failed");
        return;
    }
    if (child_process == 0)
    {
        pid_t child_pid = getpid();
        pid_t original_parent_pid = getppid();
        printf("\n========== Orphan Process Demonstration ==========\n");
        printf("Child PID: %d\n", child_pid);
        printf("Child PPID before parent exits: %d\n", original_parent_pid);
        sleep(3);
        printf("Child PPID after parent exits: %d\n", getppid());
        printf("Child process is now orphaned and has been re-parented.\n");
        return;
    }
    printf("\nCLI process is terminating.\n");
    printf("The orphan demonstration child will continue running.\n");
}

static int launch_application(int choice)
{
    pid_t child_process = fork();
    if (child_process < 0)
    {
        perror("fork failed");
        return -1;
    }
    if (child_process == 0)
    {
        printf("\nChild process started.\n");
        printf("Child PID: %d\n", getpid());
        printf("Child PPID: %d\n", getppid());
        switch (choice)
        {
        case 1:
        {
            char *arguments[] = {"/usr/bin/nano", NULL};
            execve("/usr/bin/nano", arguments, environ);
            break;
        }
        case 2:
        {
            char *arguments[] = {"/usr/bin/vim", NULL};
            execve("/usr/bin/vim", arguments, environ);
            break;
        }
        case 3:
        {
            char *arguments[] = {"/usr/bin/top", NULL};
            execve("/usr/bin/top", arguments, environ);
            break;
        }
        case 4:
        {
            char *arguments[] = {"/usr/bin/bc", NULL};
            execve("/usr/bin/bc", arguments, environ);
            break;
        }
        case 5:
        {
            char *arguments[] = {"/usr/bin/less", NULL};
            execve("/usr/bin/less", arguments, environ);
            break;
        }
        default:
            fprintf(stderr, "Invalid application selection\n");
            _exit(EXIT_FAILURE);
        }
        perror("execve failed");
        _exit(EXIT_FAILURE);
    }
    printf("\nParent CLI process:\n");
    printf("Parent PID: %d\n", getpid());
    printf("Child PID: %d\n", child_process);
    int child_status;
    if (waitpid(child_process, &child_status, 0) < 0)
    {
        perror("waitpid failed");
        return -1;
    }
    if (WIFEXITED(child_status))
        printf("Child process %d exited with status %d.\n", child_process, WEXITSTATUS(child_status));
    else if (WIFSIGNALED(child_status))
        printf("Child process %d was terminated by signal %d.\n", child_process, WTERMSIG(child_status));
    return 0;
}

int main()
{
    int choice;
    while (1)
    {
        display_cli();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            fprintf(stderr, "Invalid input.\n");
            int input_character;
            while ((input_character = getchar()) != '\n' && input_character != EOF)
            {
            }
            continue;
        }
        switch (choice)
        {
        case 1:
            printf("Selected Nano Text Editor.\n");
            break;
        case 2:
            printf("Selected Vim Text Editor.\n");
            break;
        case 3:
            printf("Selected Process Monitor.\n");
            break;
        case 4:
            printf("Selected Calculator.\n");
            break;
        case 5:
            printf("Selected File Pager.\n");
            break;
        case 0:
            demonstrate_orphan_process();
            return EXIT_SUCCESS;
        default:
            printf("Invalid selection. Please choose 1-5.\n");
            continue;
        }
        if (launch_application(choice) < 0)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}