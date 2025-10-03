#ifndef __NAHO_MEM_H
#define __NAHO_MEM_H

#include <stddef.h>

/* physical memory */

void * physmem_alloc_block(void);
void physmem_free_block(void * addr);

void * physmem_alloc_blocks(size_t size);
void physmem_free_blocks(void * addr, size_t size);

/* heap memory */

void * kmalloc(size_t req_size);
void * krealloc(void * p, size_t size);
void * kcalloc(size_t nobj, size_t size);
void kfree(void * ptr);

void init_physmem_allocator(void);

#endif
