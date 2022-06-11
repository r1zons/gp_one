section .bss

ar: resq 512
spr: resw 8 * 64





section .text
bits 64                     ;64bit mode

global f

convert:
    ;rax, rdx - free
    ;rax  is ans, rdx is buff
    mov rdx, rdi
    mov rax, 0xff00ff00ff00ff00
    and rdx, rax
    shr rdx, 8
    mov rax, rdi
    mov rdi, 0x00ff00ff00ff00ff
    and rax, rdi
    shl rax, 8
    or rax, rdx
    
    ret



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
        mov ch, cl
        mov rax, r8
        mov cl, al
        xor rax, rax
        mov al, ch
        mov r8, qword[rdx + rax * 8]
        add rdx, 8 * 64
        mov r9, qword[rdx + rax * 8]
        and r8, r9
        mov r10, rdx
        mov r11, rdi
        ;r8 - color & alpha
        ;r9 - alpha
        mov rdi, r8
        call convert
        mov r8, rax
        shl r8, cl
        mov rdi, r9
        call convert
        mov r9, rax
        shl r9, cl
        mov rdi, qword[r11 + rsi * 2]
        call convert
        and r9, rax
        xor r8, r9
        mov rdi, r8
        call convert
        xor qword[r11 + rsi * 2], rax
        xor eax, eax
        mov al, ch
        mov r8, qword[r10 + rax * 8]
        add rdx, 8 * 64
        mov r9, qword[r10 + rax * 8]
        and r8, r9 

        mov r10, rcx
        mov rcx, 64
        sub rcx, r10
        add rsi, 4

        mov rdi, r8
        call convert
        mov r8, rax
        shl r8, cl
        mov rdi, r9
        call convert
        mov r9, rax
        shl r9, cl
        mov rdi, qword[r11 + rsi * 2]
        call convert
        and r9, rax
        xor r8, r9
        mov rdi, r8
        call convert
        xor qword[r11 + rsi * 2], rax


        xor rax, rax






    ret






global main


main:
    mov rbp, rsp; for correct debugging
    mov qword[ar], 0
    mov qword[ar + 8], 0
    mov dword[spr], 0xabcdefab
    mov dword[spr + 4], 0xcdefabcd
    mov qword[spr + 8 * 64], 0xffffffffffffffff
    mov rdi, ar
    mov rsi, 0
    mov rdx, spr
    mov rcx, 0
    mov r8, 1
    call f
    xor RAX, RAX
    ret