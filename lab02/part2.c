#include <unistd.h>      // For standard definitions
#include <sys/syscall.h> // For architecture-independent system call macros

int main()
{
    const char *message = "Level 2: Invoking system call using generic glibc syscall() function.\n";

    /*
     * Call the generic glibc syscall function.
     * Arguments:
     *   SYS_write      - Architecture-independent macro resolving to the system call number
     *   1              - Argument 1: File descriptor for stdout
     *   message        - Argument 2: Pointer to your text buffer
     *   69             - Argument 3: Exact count of bytes to print
     */
    syscall(SYS_write, 1, message, 70);
    return 0;
}