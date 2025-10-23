const char* message = "This is a message that I have made!!!";

unsigned long len(const char* str) {
    unsigned long len;
    asm (
        "xor %%rax, %%rax\n" // Set rax to 0
        ".loop_%=:\t"
        "movb (%[str], %%rax, 1), %%bl\n\t" // move the next byte of the string to bl
        "cmpb $0, %%bl\n\t" // Check if it's \0
        "je .done_%=\n\t"
        "inc %%rax\n\t" // Increment rax
        "jmp .loop_%=\n"
        ".done_%=:\t"
        "movq %%rax, %[len]" // Move the rax register to the result variable
        : [len] "=r" (len)
        : [str] "r" (str)
        : "%bl", "%rax"
    );

    return len;
}


int main() {
    asm (
        // Call the function `len` with parameter `message`
        // same as `%rcx = len(message)`
        "movq %[msg], %%rdi\n\t"
        "callq len\n\t"
        "movq %%rax, %%rcx\n\t"

        "movq $1, %%rax\n\t" // Write syscall
        "movq $1, %%rdi\n\t" // 1 means stdout meaning the terminal
        "movq %[msg], %%rsi\n\t" // The address of the message
        "movq %%rcx, %%rdx\n\t" // The length of the message
        "syscall"
        :
        : [msg] "r"  (message)
        : "%rax", "%rdi", "%rsi", "%rdx", "%rcx"
    );
}
