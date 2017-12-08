#ifndef CLZ_H
#define	CLZ_H

#include <stdint.h>

#if defined(recursive)

//static inline __attribute((always_inline))
unsigned clz(uint32_t x)
{
    __asm__ __volatile__(
        ".intel_syntax noprefix\n\t"
        "mov rsi, 16\n\t"
        "mov rdx, 0\n\t"
        "test rdi, rdi\n\t"
        "je end\n\t"
        "start:\n\t"
        "test rsi,rsi\n\t"
        "je end\n\t"
        "mov r9, rdi\n\t"
        "mov rcx, rsi\n\t"
        "shr r9, cl\n\t"
        "test r9, r9\n\t"
        "je nxt\n\t"
        "mov rdi, r9\n\t"
        "shr rsi, 1\n\t"
        "jmp start\n\t"
        "nxt:\n\t"
        "add rdx, rsi\n\t"
        "shr rsi, 1\n\t"
        "jmp start\n\t"
        "end:\n\t"
        "mov rax, rdx\n\t"
        ".att_syntax\n\t"
    );
    register int i asm("rax");
    return i;
}

#elif defined(iteration)

static inline __attribute((always_inline))
unsigned clz(uint32_t x)
{
    // *INDENT-OFF*
    int n = 32, c = 16;
    do {
        uint32_t y = x >> c;
        if (y) { n -= c; x = y; }
        c >>= 1;
    } while (c);
    return (n - x);
    // *INDENT-ON*
}

#elif defined(byte)

static inline __attribute((always_inline))
unsigned clz(uint32_t x)
{
    // *INDENT-OFF*
    if (x == 0) return 32;
    int n = 1;
    if ((x >> 16) == 0) { n += 16; x <<= 16; }
    if ((x >> 24) == 0) { n += 8; x <<= 8; }
    if ((x >> 28) == 0) { n += 4; x <<= 4; }
    if ((x >> 30) == 0) { n += 2; x <<= 2; }
    n = n - (x >> 31);
    return n;
    // *INDENT-ON*
}

#elif defined(binary)

static inline __attribute((always_inline))
unsigned clz(uint32_t x)
{
    // *INDENT-OFF*
    if (x == 0) return 32;
    int n = 0;
    if (x <= 0x0000FFFF) { n += 16; x <<= 16; }
    if (x <= 0x00FFFFFF) { n += 8; x <<= 8; }
    if (x <= 0x0FFFFFFF) { n += 4; x <<= 4; }
    if (x <= 0x3FFFFFFF) { n += 2; x <<= 2; }
    if (x <= 0x7FFFFFFF) { n += 1; x <<= 1; }
    return n;
    // *INDENT-ON*
}

#elif defined(harley)
unsigned clz(uint32_t x)
{
    // *INDENT-OFF*
    static uint8_t const Table[] = {
        0xFF,    0, 0xFF,   15, 0xFF,    1,   28, 0xFF,
          16, 0xFF, 0xFF, 0xFF,    2,   21,   29, 0xFF,
        0xFF, 0xFF,   19,   17,   10, 0xFF,   12, 0xFF,
        0xFF,    3, 0xFF,    6, 0xFF,   22,   30, 0xFF,
          14, 0xFF,   27, 0xFF, 0xFF, 0xFF,   20, 0xFF,
          18,    9,   11, 0xFF,    5, 0xFF, 0xFF,   13,
          26, 0xFF, 0xFF,    8, 0xFF,    4, 0xFF,   25,
        0xFF,    7,   24, 0xFF,   23, 0xFF,   31, 0xFF,
    };
    // *INDENT-ON*

    __asm__ __volatile__(
        ".intel_syntax noprefix\n\t"
        /*
              "mov rax, 0\n\t"
              "mov rdi, 0\n\t"
              "syscall\n\t"
        */
        "mov eax, edi\n\t"
        "shr edi, 1\n\t"
        "or eax, edi\n\t"
        "mov edi, eax\n\t"
        "shr edi, 2\n\t"
        "or eax, edi\n\t"
        "mov edi, eax\n\t"
        "shr edi, 4\n\t"
        "or eax, edi\n\t"
        "mov edi, eax\n\t"
        "shr edi, 8\n\t"
        "or eax, edi\n\t"
        "mov edi, eax\n\t"
        "shr edi, 16\n\t"
        "or eax, edi\n\t"
        "mov edi, eax\n\t"
        "shl eax, 3\n\t"
        "sub eax, edi\n\t"
        "mov edi, eax\n\t"
        "shl eax, 8\n\t"
        "sub eax, edi\n\t"
        "mov edi, eax\n\t"
        "shl eax, 8\n\t"
        "sub eax, edi\n\t"
        "mov edi, eax\n\t"
        "shl eax, 8\n\t"
        "sub eax, edi\n\t"
        "shr eax, 26\n\t"
        ".att_syntax\n\t"
    );
    register uint8_t  i asm("al");
    return 31 - Table[i];
}
#endif

#endif
