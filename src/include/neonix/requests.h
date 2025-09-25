#ifndef __NEONIX_BOOT_REQUESTS_H
#define __NEONIX_BOOT_REQUESTS_H

#include <neonix/compiler.h>
#include <lib/limine.h>

#define LIMINE_REQUEST __used __section(".limine_requests")

/* requests */
/* not sure why, but the RSDP is not accessible in base revision 3 */
static volatile LIMINE_REQUEST LIMINE_BASE_REVISION(2);

static volatile struct limine_paging_mode_request LIMINE_REQUEST limine_paging_request = {
    .id = LIMINE_PAGING_MODE_REQUEST,
    .revision = 0,
    .mode = LIMINE_PAGING_MODE_X86_64_4LVL
};

static volatile struct limine_rsdp_request LIMINE_REQUEST limine_rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

static volatile struct limine_hhdm_request LIMINE_REQUEST limine_hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

static volatile struct limine_kernel_address_request LIMINE_REQUEST limine_krnl_addr_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

static volatile struct limine_memmap_request LIMINE_REQUEST limine_mmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static volatile struct limine_framebuffer_request LIMINE_REQUEST limine_fb_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

/* start & end markers */

static volatile __used __section(".limine_requests_start") LIMINE_REQUESTS_START_MARKER;
static volatile __used __section(".limine_requests_end") LIMINE_REQUESTS_END_MARKER;

#endif 
