#ifndef __X86_64_IDT_H
#define __X86_64_IDT_H

#include <neonix/compiler.h>
#include <stdint.h>

struct idtr {
    uint16_t limit;
    uint64_t addr;
} __packed;

struct intr_gate {
    uint16_t addr16;
    uint16_t segm;
    uint8_t ist;
    uint8_t flags;
    uint16_t addr32;
    uint32_t addr64;
    uint32_t reserved;
} __packed;

#define INTR_GATE_TYPE_NORMAL 0x0E
#define INTR_GATE_TYPE_FATAL  0x0F
#define INTR_GATE_DPL_RING3   (3 << 5)
#define INTR_GATE_PRESENT     (1 << 7)

/* trap interrupts */

#define X86_TRAP_DE		    0	  /* Divide-by-zero */
#define X86_TRAP_DB		    1	  /* Debug */
#define X86_TRAP_NMI	    2	  /* Non-maskable Interrupt */
#define X86_TRAP_BP		    3	  /* Breakpoint */
#define X86_TRAP_OF		    4	  /* Overflow */
#define X86_TRAP_BR		    5	  /* Bound Range Exceeded */
#define X86_TRAP_UD		    6	  /* Invalid Opcode */
#define X86_TRAP_NM		    7	  /* Device Not Available */
#define X86_TRAP_DF		    8	  /* Double Fault */
#define X86_TRAP_TS		    10	  /* Invalid TSS */
#define X86_TRAP_NP		    11	  /* Segment Not Present */
#define X86_TRAP_SS		    12	  /* Stack Segment Fault */
#define X86_TRAP_GP		    13	  /* General Protection Fault */
#define X86_TRAP_PF		    14	  /* Page Fault */
#define X86_TRAP_SPURIOUS	15	  /* Spurious Interrupt */
#define X86_TRAP_MF		    16	  /* x87 Floating-Point Exception */
#define X86_TRAP_AC		    17	  /* Alignment Check */
#define X86_TRAP_MC		    18	  /* Machine Check */
#define X86_TRAP_XF		    19	  /* SSE Exception */
#define X86_TRAP_VE		    20	  /* Virtualization Exception */
#define X86_TRAP_CP		    21	  /* Control Protection Exception */
#define X86_TRAP_VC		    29	  /* VMM Communication Exception */

struct intr_stack_frame {
    uint64_t ds;
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rbx, rdx, rcx, rax;
    uint64_t intr, error_code, rip, cs, eflags, rsp, ss;
};

typedef void (*intr_handler_func)(struct intr_stack_frame *);

#define INTR_STUB(num) extern void intr_##num()

INTR_STUB(0);
INTR_STUB(1);
INTR_STUB(2);
INTR_STUB(3);
INTR_STUB(4);
INTR_STUB(5);
INTR_STUB(6);
INTR_STUB(7);
INTR_STUB(8);
INTR_STUB(9);
INTR_STUB(10);
INTR_STUB(11);
INTR_STUB(12);
INTR_STUB(13);
INTR_STUB(14);
INTR_STUB(15);
INTR_STUB(16);
INTR_STUB(17);
INTR_STUB(18);
INTR_STUB(19);
INTR_STUB(20);
INTR_STUB(21);
INTR_STUB(22);
INTR_STUB(23);
INTR_STUB(24);
INTR_STUB(25);
INTR_STUB(26);
INTR_STUB(27);
INTR_STUB(28);
INTR_STUB(29);
INTR_STUB(30);
INTR_STUB(31);
INTR_STUB(32);
INTR_STUB(33);
INTR_STUB(34);
INTR_STUB(35);
INTR_STUB(36);
INTR_STUB(37);
INTR_STUB(38);
INTR_STUB(39);
INTR_STUB(40);
INTR_STUB(41);
INTR_STUB(42);
INTR_STUB(43);
INTR_STUB(44);
INTR_STUB(45);
INTR_STUB(46);
INTR_STUB(47);

static uint64_t intr_stubs[] = {
    (uint64_t)intr_0,
    (uint64_t)intr_1,
    (uint64_t)intr_2,
    (uint64_t)intr_3,
    (uint64_t)intr_4,
    (uint64_t)intr_5,
    (uint64_t)intr_6,
    (uint64_t)intr_7,
    (uint64_t)intr_8,
    (uint64_t)intr_9,
    (uint64_t)intr_10,
    (uint64_t)intr_11,
    (uint64_t)intr_12,
    (uint64_t)intr_13,
    (uint64_t)intr_14,
    (uint64_t)intr_15,
    (uint64_t)intr_16,
    (uint64_t)intr_17,
    (uint64_t)intr_18,
    (uint64_t)intr_19,
    (uint64_t)intr_20,
    (uint64_t)intr_21,
    (uint64_t)intr_22,
    (uint64_t)intr_23,
    (uint64_t)intr_24,
    (uint64_t)intr_25,
    (uint64_t)intr_26,
    (uint64_t)intr_27,
    (uint64_t)intr_28,
    (uint64_t)intr_29,
    (uint64_t)intr_30,
    (uint64_t)intr_31,
    (uint64_t)intr_32,
    (uint64_t)intr_33,
    (uint64_t)intr_34,
    (uint64_t)intr_35,
    (uint64_t)intr_36,
    (uint64_t)intr_37,
    (uint64_t)intr_38,
    (uint64_t)intr_39,
    (uint64_t)intr_40,
    (uint64_t)intr_41,
    (uint64_t)intr_42,
    (uint64_t)intr_43,
    (uint64_t)intr_44,
    (uint64_t)intr_45,
    (uint64_t)intr_46,
    (uint64_t)intr_47,
};

void intr_add_handler(uint8_t intr, intr_handler_func func);
void sys_idt_init(void);

#endif
