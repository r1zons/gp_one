
section .text
bits 64                     ;64bit mode

global f

f:
    ;rdi array
    ;rsi index
    ;rdx struct pointer 
    ;rcx sprite y
    ;r8 xMod16
    cmp r8, 0
    jne off
        mov r8, qword[rdx + rcx * 8]
        add rdx, 8 * 64
        mov r9, qword[rdx + rcx * 8]
        and r8, r9
        and r9, qword[rdi + rsi * 2]
        xor r8, r9
        xor qword[rdi + rsi * 2], r8
        ret
    off:
        ;r11 - rcx copy
        ;r10 color & alpha
        ;r9 alpha
        mov r11, rcx
        mov rcx, r8
        mov r8, qword[rdx + r11 * 8]
        add rdx, 8 * 64
        mov r9, qword[rdx + r11 * 8]
        and r8, r9 
        ;r8 - color & alpha
        ;r9 - alpha
        mov rax, r8
        mov rdx, r9
        shr rax, cl
        shr rdx, cl
        and rdx, qword[rdi + rsi * 2]
        xor rax, rdx
        xor qword[rdi + rsi * 2], rax

        mov r11, rcx
        mov rcx, 64
        sub rcx, r11
        add rsi, 4

        mov rax, r8
        mov rdx, r9
        shl rax, cl
        shl rdx, cl
        and rdx, qword[rdi + rsi * 2]
        xor rax, rdx
        xor qword[rdi + rsi * 2], rax

    ret



