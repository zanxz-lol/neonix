#include "arch/operations.h"
#include <neonix/limits.h>
#include <neonix/printk.h>
#include <neonix/errno.h>
#include <neonix/proc.h>
#include <neonix/mem.h>
#include <lib/list.h>
#include <lib/bool.h>
#include <lib/string.h>

static struct list sched_queue = {NULL, NULL};
static bool sched_enabled = false;

static void __noreturn neonix_idle(void) {
    sched_enabled = true;
    printk("NEONIX idle thread has been successfully created.\n");   
    for(;;);
}

bool is_sched_enabled(void) {
    return sched_enabled;
}

struct proc * get_current_proc(void) {
    return (struct proc *)sched_queue.data;
}

struct proc * find_next_proc(void) {
    size_t procs = list_get_elems(&sched_queue);
    if (procs == 1) {
        return sched_queue.data;
    }
    return NULL;
}

static pid_t pid_allocate(void) {
    static pid_t last_pid = 0;
    pid_t alloc_pid = last_pid++;
    return (alloc_pid % PID_LIMIT);
}

struct proc * create_kernel_proc(char * name, void * start) {
    struct proc * current = (struct proc *)kmalloc(sizeof(struct proc));
    if (current == NULL) {
        return NULL;
    }
    if (name == NULL || start == NULL) {
        kfree(current);
        return NULL;
    }
    strncpy(current->name, name, NAME_MAX - 1);
    current->gid = 0;
    current->uid = 0;
    current->pid = pid_allocate();
    current->ppid = 0;
    current->state = PROC_CREATED;
    current->time_quantum = 3;
    current->cpu_time_left = current->time_quantum;
    void * stack = kmalloc(4096);
    if (stack == NULL) {
        kfree(current);
        return NULL;
    }
    arch_setup_context(&current->context, start, stack);
    arch_dbg_putc('$');
    return current;
}

static void add_proc_to_queue(struct proc * proc) {
    if (proc == NULL) {
        return;
    }
    list_append(&sched_queue, proc);
    printk("Added \"%s\" (pid %d) to scheduler queue\n", proc->name, proc->pid);
}

void create_idle_proc(void) {
    struct proc * idle = create_kernel_proc("neonix-idle", neonix_idle);
    if (idle == NULL) {
        panic("Failed to create kernel idle process\n");
    }
    add_proc_to_queue(idle);
    idle->state = PROC_ALIVE;
    arch_first_context_switch(&idle->context);
}
