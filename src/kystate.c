#include "includes/kystate.h"
#include "includes/kymem.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

ky_state_t* ky_state_new() {
    /* use malloc only here as state doesn't exist yet and
       ky_alloc may need an already existing state */
    ky_state_t* k = cast(ky_state_t*, malloc(sizeof(ky_state_t)));
    k->m_total = sizeof(ky_state_t);
    k->gc_on = KFALSE;
    k->n_all = 0;
    k->status = KY_GOOD;
    ky_htable_init(k);
    return k;
}

void ky_state_free(ky_state_t* k) {
    ky_objects_free(k);
    if (k->buffer) ky_alloc(k, k->buffer, 0);
    if (k->htable.hash) ky_alloc(k, k->htable.hash, 0);
    k = ky_alloc(k, k, 0);
}

/**
 * frees ALL objects in o_all, including those that aren't
 * GC-able. doesn't decrement m_total.
 * only to be used in emergency or on exit
 **/
void ky_objects_free(ky_state_t *k) {
    ky_object_t *o = k->o_all;
    while (o != NULL) {
        ky_object_t *next = o->next;
        ky_alloc(k, o, 0);
        o = next;
    }
}

void ky_buffer_grow(ky_state_t *k, size_t size) {
    if (!k->buffer) {
        k->buffer = ky_alloc(k, NULL, size); 
    } else if (k->m_buffer < size) {
        k->buffer = ky_alloc(k, k->buffer, size);
    }
}

void ky_buffer_putch(ky_state_t *k, size_t index, char c) {
    if (index >= k->m_buffer) {
        ky_buffer_grow(k, KBUFF_GROW_SIZE(k));
    }
    k->buffer[index] = c;
}

void ky_errorf(ky_state_t* k, kstatus err, const char *fmt, ...) {
    va_list vl;
    char str[KY_MAX_ERR_FMT];
    va_start(vl, fmt);
    vsprintf(str, fmt, vl);
    va_end(vl);
    ky_error(k, err, str);
}

void ky_error(ky_state_t* k, kstatus err, const char *str) {
    KUNUSED(k);
    /* TODO : better error recovery - e.g jump; for now just panic & exit */
    printf("%s\n", KY_OUT_ERR);
    if (str) {
        printf("%s\n", str);
    } else {
        switch(err) {
            case KY_ERR_RUN: printf("uncaught runtime error\n");    break;
            case KY_ERR_SYN: printf("uncaught syntax error\n");     break;
            case KY_ERR_MEM: printf("uncaught memory exception\n"); break;
            default:         printf("unknown error\n");             break;
        }
    }
    exit(EXIT_FAILURE);
}

void ky_debug(ky_state_t* k, const char *str) {
    KUNUSED(k);
    printf("╔═[ 🚀 debug ]═════\n║ %s\n", str);
}

void ky_debugf(ky_state_t* k, const char *fmt, ...) {
    va_list vl;
    char str[KY_MAX_ERR_FMT];
    va_start(vl, fmt);
    vsprintf(str, fmt, vl);
    va_end(vl);
    ky_debug(k, str);
}