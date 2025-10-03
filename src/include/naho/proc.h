#ifndef __NAHO_PROC_H
#define __NAHO_PROC_H

#include <arch/sched.h>
#include <naho/limits.h>
#include <naho/types.h>
#include <lib/bool.h>
#include <stddef.h>

#define PID_LIMIT 4194304

enum state {
    PROC_UNKNOWN,
    PROC_CREATED,
    PROC_ALIVE,
    PROC_DEAD,
    PROC_ZOMBIE
};

struct proc {
    char name[NAME_MAX];
    uid_t uid;
    gid_t gid;
    pid_t ppid; /* parent pid */
    pid_t pid; /* thread pid */
    size_t time_quantum;
    size_t cpu_time_left;
    enum state state;
    struct arch_context context;
};

bool is_sched_enabled(void);

struct proc * get_current_proc(void);
struct proc * find_next_proc(void);
struct thread * create_kernel_thread(const char * name, void * start);
void create_idle_proc(void);

#endif
