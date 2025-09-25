#include <neonix/mem.h>
#include <neonix/errno.h>
#include <neonix/printk.h>
#include <lib/list.h>
#include <stddef.h>

void list_init(struct list * list) {
    if (list == NULL) {
        return;
    }
    list->data = NULL;
    list->next = NULL;
}

void list_append(struct list * head, void * data) {
    if (head == NULL) {
        return;
    }
    /* first append */
    if (head->data == NULL && head->next == NULL) {
        head->data = data;
        return;
    }
    struct list * curr = head;
    while(curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = (struct list *)kmalloc(sizeof(struct list));
    curr->next->data = data;
    curr->next->next = NULL;
}

size_t list_get_elems(struct list * head) {
    if (head == NULL) {
        return 0;
    }
    size_t elems = 0;
    struct list * curr = head;
    while (curr != NULL) {
        if (curr->data != NULL) {
            elems++;
        }
        curr = curr->next;
    }
    return elems;
}
