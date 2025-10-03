section .text
    global sys_enable_avx

 sys_enable_avx:
    push rax
    push rcx
    push rdx

    xor rcx, rcx
    ; get xcr0
    xgetbv
    or eax, (1 << 0) | (1 << 1) | (1 << 2)
    ; set xcr0
    xsetbv

    pop rdx
    pop rcx
    pop rax
    ret
