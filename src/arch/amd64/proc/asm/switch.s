section .text
    global perform_first_context_switch
    extern intr_stub_end

perform_first_context_switch:
    mov rax, [rsp + 4]
    mov rsp, rax
    jmp intr_stub_end
