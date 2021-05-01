#include "includes/kyobject.h"
#include "includes/kymem.h"
#include "includes/kystring.h"

#include <string.h>

ky_object_t* ky_object_new(ky_state_t *k, kbyte type, size_t size) {
    /* create the object with larger of given size or size of ky_object_t */
    size = (size > sizeof(ky_object_t)) ? size : sizeof(ky_object_t);
    ky_object_t *o = cast(ky_object_t*, ky_alloc(k, NULL, size));
    o->type = type;        /* set type */
    o->gcm = KY_GCM_WHITE; /* set as initially collectable */
    o->next = k->gco;      /* add to front of gco list */
    k->gco = o;
    k->n_gco++;            /* increase memory counts as appropriate */
    k->m_total += size;
    k->m_gco += size;
    return o;
}

ky_ident_node_t* ky_ident_new(ky_state_t* k, ky_string_t* s) {
    ky_ident_node_t* node = kmem_new(k, ky_ident_node_t);
    k->m_total += sizeof(ky_ident_node_t);
    node->ident = s;
    node->child = NULL;
    node->parent = NULL;
    node->next = NULL;
    return node;
}

ky_ident_node_t* ky_ident_find_or_create(ky_state_t* k, ky_ident_node_t** previous, ky_string_t* s) {
    ky_ident_t* node;
    if ((node = ky_ident_find(k, *previous, s)))
        return node;
    node = ky_ident_new(k, s);
    ky_ident_insert(k, previous, node);
    return node;
}

ky_ident_node_t* ky_ident_find(ky_state_t* k, ky_ident_node_t* first, ky_string_t* s) {
    while (first) {
        if (first->ident->size == s->size && memcmp(first->ident->cs, s->cs, s->size) == 0)
            return first;
        first = first->next;
    }
    return NULL;
}

void ky_ident_insert(ky_state_t* k, ky_ident_node_t** previous, ky_ident_node_t* node) {
    if (*previous)
        node->next = *previous;
    *previous = node;
}

void ky_ident_free(ky_state_t* k, ky_ident_node_t* node) {
    while (node) {
        ky_ident_node_t *next = node->next;
        if (node->child)
            ky_ident_free(k, node->child);
        kmem_free(k, node);
        k->m_total -= sizeof(ky_ident_node_t);
        node = next;
    }
}