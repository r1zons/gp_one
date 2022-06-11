
section .text
bits 64                     ;64bit mode

global load_of_0
global load_stuff




load_of_0:
    ;rdi array
    ;rsi index
    ;rdx struct pointer 
    ;rcx sprite y
    ;r8 xMod16
    mov r8, qword[rdx + rcx * 8]
    add rdx, 8 * 64
    mov r9, qword[rdx + rcx * 8]
    and r8, r9
    and r9, qword[rdi + rsi * 2]
    xor r8, r9
    xor qword[rdi + rsi * 2], r8
    ret


load_stuff:
    ;rdi array
    ;rsi index
    ;rdx color
    ;rcx alpha
    mov r8, rcx
    mov r9, rdi
    mov r10, rsi
    mov rcx, 4
    add r10, 4
    loop1:
        mov ax, word[r8 + rcx * 2]
        mov di, word[rdx + rcx * 2]
        xor di, ax
        and ax, word[r9 + r10 * 2]
        dec r10
        xor di, ax
        xor word[r9 + r10 * 2], di

    loop loop1
    xor rax, rax
    ret


