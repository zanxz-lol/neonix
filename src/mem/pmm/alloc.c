#include <arch/operations.h>
#include <neonix/kernel.h>
#include <neonix/printk.h>
#include <neonix/math.h>
#include <neonix/mem.h>
#include <lib/string.h>
#include <lib/common.h>
#include <lib/bitmap.h>
#include <lib/limine.h>

static struct bitmap physmem_bitmap;
static size_t physmem_total_blocks = 0;

#define PHYSMEM_BLOCK_SIZE 4096

static void physmem_set_region(uint64_t addr, uint64_t size) {
    size_t sz = DIV_CEILING(size, PHYSMEM_BLOCK_SIZE);
    size_t block = addr / PHYSMEM_BLOCK_SIZE;
    for(size_t i = 0; i < sz; i++) {
        bitmap_set(&physmem_bitmap, block + i);
    }
}

static void physmem_unset_region(uint64_t addr, uint64_t size) {
    size_t sz = DIV_CEILING(size, PHYSMEM_BLOCK_SIZE);
    size_t block = addr / PHYSMEM_BLOCK_SIZE;
    for(size_t i = 0; i < sz; i++) {
        bitmap_unset(&physmem_bitmap, block + i);
    }
}

void * physmem_alloc_block(void) {
    int bit = bitmap_find_first_free_bit(&physmem_bitmap);
    if (bit < 0) {
        return NULL;
    }
    bitmap_set(&physmem_bitmap, bit);
    return (void *)((uint64_t)bit * PHYSMEM_BLOCK_SIZE);
}

void * physmem_alloc_blocks(size_t size) {
    int bit = bitmap_find_free_bits(&physmem_bitmap, size);
    if (bit < 0) {
        return NULL;
    }
    for(size_t i = 0; i < size; i++) {
        bitmap_set(&physmem_bitmap, bit + i);
    }
    return (void *)((uint64_t)bit * PHYSMEM_BLOCK_SIZE);
}

void physmem_free_block(void * addr) {
    size_t block = (uint64_t)addr / PHYSMEM_BLOCK_SIZE;
    bitmap_unset(&physmem_bitmap, block);
}

void physmem_free_blocks(void * addr, size_t size) {
    size_t block = (uint64_t)addr / PHYSMEM_BLOCK_SIZE;
    for(size_t i = 0; i < size; i++) {
        bitmap_unset(&physmem_bitmap, block + i);
    }
}

void init_physmem_allocator(void) {
    struct limine_memmap_entry * entry = NULL;
    physmem_total_blocks = DIV_CEILING(boot_info.avail_mem, PHYSMEM_BLOCK_SIZE);
    physmem_bitmap.size = DIV_CEILING(physmem_total_blocks, 8);
    /* check if there's enough mem */
    for(size_t i = 0; i < boot_info.mmap_entries; i++) {
        if (boot_info.mmap[i]->type == LIMINE_MEMMAP_USABLE) {
            if (physmem_bitmap.size < boot_info.mmap[i]->length) {
                entry = boot_info.mmap[i];
                break;
            }
        }
    }
    printk("Total memory: %u bytes\n", boot_info.total_mem);
    printk("Available memory: %u bytes\n", boot_info.avail_mem);
    if (entry == NULL) {
        panic("Not enough memeory to setup physical memory bitmap\n");
    }
    physmem_bitmap.data = (uint8_t *)(boot_info.virt_offset + entry->base);
    memset(physmem_bitmap.data, 0, physmem_bitmap.size);
    printk("Physical memory bitmap located @ 0x%p\n", physmem_bitmap.data);
    printk("Physical memory bitmap size: %u bytes\n", physmem_bitmap.size);
    for(size_t i = 0; i < boot_info.mmap_entries; i++) {
        if (boot_info.mmap[i]->type == LIMINE_MEMMAP_USABLE) {
            physmem_unset_region(boot_info.mmap[i]->base, boot_info.mmap[i]->length);
        } else {
            physmem_set_region(boot_info.mmap[i]->base, boot_info.mmap[i]->length);
        }
    }
    physmem_set_region((uint64_t)(physmem_bitmap.data - boot_info.virt_offset), physmem_bitmap.size);
    /* should stay reserved (no null references) */
    bitmap_set(&physmem_bitmap, 0);
}
