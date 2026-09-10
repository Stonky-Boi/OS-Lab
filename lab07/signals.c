#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

static volatile sig_atomic_t received_signal = 0;

static void signal_handler(int signal_number)
{
    received_signal = signal_number;
}

static int set_signal_handler(int signal_number, void (*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    if (sigemptyset(&action.sa_mask) < 0)
        return -1;
    action.sa_flags = 0;
    return sigaction(signal_number, &action, NULL);
}

int main()
{
    printf("Signal Demonstration\n");
    printf("====================\n");
    printf("Process PID: %d\n\n", getpid());
    if (set_signal_handler(SIGUSR1, signal_handler) < 0)
    {
        perror("Failed to handle SIGUSR1");
        return EXIT_FAILURE;
    }
    if (set_signal_handler(SIGUSR2, SIG_IGN) < 0)
    {
        perror("Failed to ignore SIGUSR2");
        return EXIT_FAILURE;
    }
    printf("SIGUSR1: Caught and handled\n");
    printf("SIGUSR2: Ignored\n");
    printf("SIGTERM: Default behavior\n");
    printf("\nSend signals from another terminal using:\n");
    printf("kill -SIGUSR1 %d\n", getpid());
    printf("kill -SIGUSR2 %d\n", getpid());
    printf("kill -SIGTERM %d\n", getpid());
    printf("\n");
    while (1)
    {
        pause();
        if (received_signal == SIGUSR1)
        {
            printf("Received SIGUSR1: signal handled by the program.\n");
            received_signal = 0;
        }
    }
    return EXIT_SUCCESS;
}