#include <unistd.h> // Standard POSIX API wrappers header
#include <string.h> // Header for strlen()

int main()
{
    // Define your message buffer
    const char *message = "Level 1: Invoking the high-level glibc write() wrapper API.\n";

    /*
     * Call the glibc wrapper function.
     * Arguments:
     *   1               - File descriptor for Standard Output (stdout)
     *   message         - Pointer to your character array
     *   strlen(message) - Number of bytes to transfer
     */
    write(1, message, strlen(message));
    return 0;
}