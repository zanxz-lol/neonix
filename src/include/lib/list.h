#ifndef __LIB_LIST_H
#define __LIB_LIST_H

#include <stddef.h>

struct list {
    void * data;
    struct list * next;
};

void list_init(struct list * list);
void list_append(struct list * list, void * data);
size_t list_get_elems(struct list * list);

#endif
