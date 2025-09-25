section .text
    global intr_stub_start
    global intr_stub_end
    extern intr_handler

intr_stub_start:
    ; save registers
    push rax
    push rcx
    push rdx
    push rbx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15  
    ; save data segment
    mov ax, ds
    push rax
    ; load new data segment to access kernel data
    mov ax, 0x30
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; transition to c code
    mov rdi, rsp
    cld
    call intr_handler
intr_stub_end:
    ; restore data segment, and other registers
    pop rax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    add rsp, 16
    ; go back to where we left off
    sti
    iretq

; 1 = dummy error code
; 0 = cpu-pushed error code
%macro DEFINE_INTR_HANDLER 2
global intr_%1
intr_%1:
  cli
%if %2 == 1
  push byte 0
%endif
  push %1
  jmp intr_stub_start
%endmacro

DEFINE_INTR_HANDLER 0, 1
DEFINE_INTR_HANDLER 1, 1
DEFINE_INTR_HANDLER 2, 1
DEFINE_INTR_HANDLER 3, 1
DEFINE_INTR_HANDLER 4, 1
DEFINE_INTR_HANDLER 5, 1
DEFINE_INTR_HANDLER 6, 1
DEFINE_INTR_HANDLER 7, 1
DEFINE_INTR_HANDLER 8, 0
DEFINE_INTR_HANDLER 9, 1
DEFINE_INTR_HANDLER 10, 0
DEFINE_INTR_HANDLER 11, 0
DEFINE_INTR_HANDLER 12, 0
DEFINE_INTR_HANDLER 13, 0
DEFINE_INTR_HANDLER 14, 0
DEFINE_INTR_HANDLER 15, 1
DEFINE_INTR_HANDLER 16, 1
DEFINE_INTR_HANDLER 17, 0
DEFINE_INTR_HANDLER 18, 1
DEFINE_INTR_HANDLER 19, 1
DEFINE_INTR_HANDLER 20, 1
DEFINE_INTR_HANDLER 21, 0
DEFINE_INTR_HANDLER 22, 1
DEFINE_INTR_HANDLER 23, 1
DEFINE_INTR_HANDLER 24, 1
DEFINE_INTR_HANDLER 25, 1
DEFINE_INTR_HANDLER 26, 1
DEFINE_INTR_HANDLER 27, 1
DEFINE_INTR_HANDLER 28, 1
DEFINE_INTR_HANDLER 29, 0
DEFINE_INTR_HANDLER 30, 0
DEFINE_INTR_HANDLER 31, 1
DEFINE_INTR_HANDLER 32, 1
DEFINE_INTR_HANDLER 33, 1
DEFINE_INTR_HANDLER 34, 1
DEFINE_INTR_HANDLER 35, 1
DEFINE_INTR_HANDLER 36, 1
DEFINE_INTR_HANDLER 37, 1
DEFINE_INTR_HANDLER 38, 1
DEFINE_INTR_HANDLER 39, 1
DEFINE_INTR_HANDLER 40, 1
DEFINE_INTR_HANDLER 41, 1
DEFINE_INTR_HANDLER 42, 1
DEFINE_INTR_HANDLER 43, 1
DEFINE_INTR_HANDLER 44, 1
DEFINE_INTR_HANDLER 45, 1
DEFINE_INTR_HANDLER 46, 1
DEFINE_INTR_HANDLER 47, 1
