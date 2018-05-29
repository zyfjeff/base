#ifndef BASE_LIST_H_
#define BASE_LIST_H_

/**
 * 列表，参考kernel上的list.h
 */

#include "base_define.h"

BASE_CPP_START

// from kernel list
typedef struct base_list_t base_list_t;

struct base_list_t {
    base_list_t             *next, *prev;
};

#define base_LIST_HEAD_INIT(name) {&(name), &(name)}
#define base_list_init(ptr) do {                \
        (ptr)->next = (ptr);                    \
        (ptr)->prev = (ptr);                    \
    } while (0)

static inline void __base_list_add(base_list_t *list,
                                   base_list_t *prev, base_list_t *next)
{
    next->prev = list;
    list->next = next;
    list->prev = prev;
    prev->next = list;
}
// list head to add it after
static inline void base_list_add_head(base_list_t *list, base_list_t *head)
{
    __base_list_add(list, head, head->next);
}
// list head to add it before
static inline void base_list_add_tail(base_list_t *list, base_list_t *head)
{
    __base_list_add(list, head->prev, head);
}
static inline void __base_list_del(base_list_t *prev, base_list_t *next)
{
    next->prev = prev;
    prev->next = next;
}
// deletes entry from list
static inline void base_list_del(base_list_t *entry)
{
    __base_list_del(entry->prev, entry->next);
    base_list_init(entry);
}
// tests whether a list is empty
static inline int base_list_empty(const base_list_t *head)
{
    return (head->next == head);
}
// move list to new_list
static inline void base_list_movelist(base_list_t *list, base_list_t *new_list)
{
    if (!base_list_empty(list)) {
        new_list->prev = list->prev;
        new_list->next = list->next;
        new_list->prev->next = new_list;
        new_list->next->prev = new_list;
        base_list_init(list);
    } else {
        base_list_init(new_list);
    }
}
// join list to head
static inline void base_list_join(base_list_t *list, base_list_t *head)
{
    if (!base_list_empty(list)) {
        base_list_t             *first = list->next;
        base_list_t             *last = list->prev;
        base_list_t             *at = head->prev;

        first->prev = at;
        at->next = first;
        last->next = head;
        head->prev = last;
    }
}

#define base_list_entry(ptr,type,mbr) ({(type*)((char*)(ptr)-offsetof(type,mbr));})

// get last
#define base_list_get_last(list,type,mbr) base_list_empty(list)?NULL:base_list_entry((list)->prev,type,mbr)

// get first
#define base_list_get_first(list,type,mbr) base_list_empty(list)?NULL:base_list_entry((list)->next,type,mbr)

#define base_list_for_each_entry(it,hdr,mbr)\
    for(it=base_list_entry((hdr)->next,__typeof__(*it),mbr);(hdr)!=&it->mbr;it=base_list_entry(it->mbr.next,__typeof__(*it),mbr))

#define base_list_for_each_entry_reverse(it,hdr,mbr)\
    for(it=base_list_entry((hdr)->prev,__typeof__(*it),mbr);(hdr)!=&it->mbr;it=base_list_entry(it->mbr.prev,__typeof__(*it),mbr))

#define base_list_for_each_entry_safe(it,n,hdr,mbr)\
    for(it=base_list_entry((hdr)->next,__typeof__(*it),mbr),n=base_list_entry(it->mbr.next,__typeof__(*it),mbr);\
            (hdr)!=&it->mbr;it=n,n=base_list_entry(n->mbr.next,__typeof__(*n),mbr))

#define base_list_for_each_entry_safe_reverse(it,n,hdr,mbr)\
    for(it=base_list_entry((hdr)->prev,__typeof__(*it),mbr),n=base_list_entry(it->mbr.prev,__typeof__(*it),mbr);\
            (hdr)!=&it->mbr;it=n,n=base_list_entry(n->mbr.prev,__typeof__(*n),mbr))

BASE_CPP_END

#endif
