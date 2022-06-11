section .text
global f

f:
    ;rdi array
    ;rsi index
    ;rdx struct pointer 
    ;rcx sprite y
    ;r8 first uint offset
    mov r9, rdx
    mov rdx, qword[r9 + rcx * 8];color
    add r9, 512
    and rdx, qword[r9 + rcx * 8];alpha
    mov r9, rdx
    shr r9, r8
    xor qword[rdi + rsi * 2], r9
    xor qword[rdi + rsi * 2], r9
    mov r9, rdx
    neg r8
    add r8, 64
    shl r9, r8
    add rdi, 8
    xor qword[rdi + rsi * 2], r9
    xor qword[rdi + rsi * 2], r9
    ret