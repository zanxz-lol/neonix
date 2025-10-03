#ifndef __NAHO_ERRNO_H
#define __NAHO_ERRNO_H

#define EOK            0    /* OK */
#define EPERM          1    /* Operation not permitted */
#define ENOENT         2    /* No such file or directory */
#define ESRCH          3    /* No such process */
#define EINTR          4    /* Interrupted system call */
#define EIO            5    /* I/O error */
#define EDEV           6    /* Device error */
#define E2BIG          7    /* Argument list too long */
#define ENOEXEC        8    /* Not executable */
#define EBADF          9    /* Bad file number */
#define ECHILD        10    /* No child processes */
#define EAGAIN        11    /* Try again */
#define ENOMEM        12    /* Out of memory */
#define EACCES        13    /* Access denied */
#define EBADADDR      14    /* Bad address */
#define ENOTBLK       15    /* Block device is required */
#define EBUSY         16    /* Device or resource is busy */
#define EEXIST        17    /* File exists */
#define EXDEV         18    /* Cross-device link */
#define ENODEV        19    /* No such device */
#define ENOTDIR       20    /* Not a directory */
#define EISDIR        21    /* Is a directory */
#define EINVAL        22    /* Invalid argument */
#define ENFILE        23    /* File table overflow */
#define EMFILE        24    /* Too many open files */
#define ENOTTY        25    /* Not a typewriter */
#define EEXECBSY      26    /* Executable file is running */
#define EFBIG         27    /* File too large */
#define ENOSPC        28    /* No space left on device */
#define ESPIPE        29    /* Illegal seek */
#define EROFS         30    /* File system is read-only */
#define EMLINK        31    /* Too many links */
#define EPIPE         32    /* Broken pipe */
#define EDOM          33    /* Math argument out of domain of func */
#define ERANGE        34    /* Math result not representable */
#define ENOSYS        35    /* System call not implemented */

static inline char * errno2str(int rc) {
    switch(rc) {
        case EOK: return "Success";
        case EPERM: return "Operation not permitted";
        case ENOENT: return "No such file or directory";
        case ESRCH: return "No such process";
        case EINTR: return "Interrupted syscall";
        case EIO: return "I/O error";
        case EDEV: return "Device error";
        case E2BIG: return "Argument list too long";
        case ENOEXEC: return "Not executable";
        case EBADF: return "Bad file descriptor";
        case ECHILD: return "No child processes";
        case EAGAIN: return "Try again";
        case ENOMEM: return "Out of memory";
        case EACCES: return "Access denied";
        case EBADADDR: return "Bad address";
        case ENOTBLK: return "Block device is required";
        case EBUSY: return "Device or resource is busy";
        case EEXIST: return "File already exists";
        case EXDEV: return "Cross-device link";
        case ENODEV: return "No such device";
        case ENOTDIR: return "Not a directory";
        case EISDIR: return "Is a directory";
        case EINVAL: return "Invalid argument";
        case ENFILE: return "Too many files opened in the system";
        case EMFILE: return "Too many files opened in the current process";
        case ENOTTY: return "Not a TTY device";
        case EEXECBSY: return "Executable is running";
        case EFBIG: return "File too large";
        case ENOSPC: return "No space left on device";
        case ESPIPE: return "Illegal seek";
        case EROFS: return "File system is read-only";
        case EMLINK: return "Too many links";
        case EPIPE: return "Broken pipe";
        case EDOM: return "Math argument out of domain of func";
        case ERANGE: return "Math result not representable";
        case ENOSYS: return "System call not implemented";
    }
    return "Unknown";
}

#define IS_ERROR(ret) \
    ((ret) > 0)

#endif
