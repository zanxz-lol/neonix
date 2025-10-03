OUTPUT_FORMAT(elf64-x86-64)
ENTRY(neonix_entry)

PHDRS {
    limine_requests PT_LOAD;
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
}

SECTIONS {
    /* neonix's load address in virtual memory */
    . = 0xffffffff80000000;
    kernel_start = .;
    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } : limine_requests

    .text : ALIGN(CONSTANT(MAXPAGESIZE)) {
        text_start = .;
        *(.text)
    } : text

    .initcall.text : ALIGN(CONSTANT(MAXPAGESIZE)) {
        initcalls_start = .;
        initcall0_start = .;
        *(.initcall0.init)
        initcall0_end = .;

        initcall1_start = .;
        *(.initcall1.init)
        initcall1_end = .;

        initcall2_start = .;
        *(.initcall2.init)
        initcall2_end = .;
        initcalls_end = .;
        text_end = .;
    } : text

    .rodata : ALIGN(CONSTANT(MAXPAGESIZE)) {
        rodata_start = .;
        *(.rodata)
        rodata_end = .;
    } : rodata

    .data : ALIGN(CONSTANT(MAXPAGESIZE)) {
        data_start = .;
        *(.data)
        data_end = .;
    } : data

    .bss : ALIGN(CONSTANT(MAXPAGESIZE)) {
        bss_start = .;
        *(.bss)
        *(COMMON)
        bss_end = .;
    } : data
    /* USELESS USELESS USELESS USELESS USELESS USELESS!! */
    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
    kernel_end = .;
}
