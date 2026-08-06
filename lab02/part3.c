#include <string.h> // For strlen()

int main()
{
    const char *message = "Level 3: Direct x86_64 inline assembly system call invocation.\n";
    size_t len = strlen(message);
    /*
     * Direct System Call via x86_64 Inline Assembly
     * Bypasses glibc C wrappers entirely.
     */
    __asm__ volatile(
        "movq $1, %%rax\n\t" // System call number 1 is 'write' on x86_64
        "movq $1, %%rdi\n\t" // File descriptor 1 is stdout
        "movq %0, %%rsi\n\t" // Pointer to the message buffer
        "movq %1, %%rdx\n\t" // Length of the message in bytes
        "syscall\n\t"        // Trigger CPU privilege transition (Ring 3 -> Ring 0)
        :
        : "r"(message), "r"(len)         // Input operands mapped to registers
        : "%rax", "%rdi", "%rsi", "%rdx" // Clobbered hardware registers
    );
    return 0;
}