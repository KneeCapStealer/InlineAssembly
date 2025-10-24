const char* message = "This is a message that I have made!!!";

unsigned int len(const char* str) {
    // Needs to be long type so it get's assigned to a 64bit register
    // This is necesarry so that the 3. line will compile
    // You can't index a 64bit address with a 32bit register
    unsigned long res;
    asm (
        "xor %[res], %[res]\n" // Set rax to 0
        ".loop_%=:\t"
        "movb (%[str], %[res], 1), %%bl\n\t" // move the next byte of the string to bl
        "cmpb $0, %%bl\n\t" // Check if it's \0
        "je .done_%=\n\t"
        "inc %[res]\n\t" // Increment rax
        "jmp .loop_%=\n"
        ".done_%=:"
        : [res] "=r" (res)
        : [str] "r" (str)
        : "%bl"
    );

    // Implicit cast from ulong -> uint
    return res;
}


int main() {
    asm (
        // Call the function `len` with parameter `message`
        // same as `%rcx = len(message)`
        "movq %[msg], %%rdi\n\t"
        "callq len\n\t"
        "movl %%eax, %%ecx\n\t"

        "movq $1, %%rax\n\t" // Write syscall
        "movq $1, %%rdi\n\t" // 1 means stdout meaning the terminal
        "movq %[msg], %%rsi\n\t" // The address of the message
        "movl %%ecx, %%edx\n\t" // The length of the message
        "syscall"
        :
        : [msg] "r"  (message)
        : "%rax", "%rdi", "%rsi", "%rdx", "%rcx"
    );
}
