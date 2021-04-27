#include "includes/kygc.h"
#include "includes/kystate.h"
#include "includes/kymem.h"

/**
 * check object references and mark them accordingly
 **/
static void ky_gc_mark(ky_state_t* k) {
    KUNUSED(k);
    /* TODO : go through and put things into grey / black categories */
}

/**
 * if eligible, collect the string at the given location
 * return pointer to the next object pointer in the GC list
 **/
static ky_object_t** ky_gc_collect_string(ky_state_t* k, ky_object_t **o, kbyte collection) {
    ky_string_t *s = ko2str(*o);
    if (!_kstr_collectable(s) && collection != KY_GC_COLLECTION_ALL) return &((*o)->next);
    ky_htable_remove(k, s);
    kmem size = ksize_str(s->size);
    k->m_gco -= size;
    k->m_total -= size;
    *o = (*o)->next;
    kmem_free(k, s);
    return o;
}

void ky_gc_collect(ky_state_t* k, kbyte collection) {
    ky_gc_mark(k);
    ky_object_t **o = &(k->gco);
    while (*o) {
        switch ((*o)->type) {
            case KY_TYPE_STRING:
                o = ky_gc_collect_string(k, o, collection);
                break;
            default:
                o = &((*o)->next);
                ky_error(k, KY_ERR_MEM, "GC cannot yet dispose of this kind of object!");
        }
    }
    ky_buffer_free(k);
}

/**
 * initiate a regular GC cycle if used memory
 * is big enough to warrant it
 **/
void ky_gc_check(ky_state_t* k) {
    if (k->gc_on && k->m_total > k->gc_thresh)
        ky_gc_collect(k, KY_GC_COLLECTION_STRICT);
}