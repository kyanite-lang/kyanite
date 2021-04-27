#include "includes/kystate.h"
#include "includes/kymem.h"
#include "includes/kygc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

ky_state_t* ky_state_new() {
    /* use malloc only here as state doesn't exist yet and
       ky_alloc may need an already existing state */
    ky_state_t* k = cast(ky_state_t*, malloc(sizeof(ky_state_t)));
    k->m_total = sizeof(ky_state_t);
    /* don't let GC run yet */
    k->gc_on = KFALSE;
    /* initialize null values for GC list and buffer */
    k->m_gco = 0;
    k->n_gco = 0;
    k->gco = NULL;
    k->m_buffer = 0;
    k->buffer = NULL;
    /* initialize hash table */
    ky_htable_init(k);
    /* set status to good */
    k->status = KY_GOOD;
    return k;
}

void ky_state_close(ky_state_t* k) {
    ky_gc_collect(k, KY_GC_COLLECTION_ALL);
    ky_buffer_free(k);
    ky_htable_free(k);
    /* use built-in free here as freeing ky_state_t
       and we can't be sure ky_alloc won't want to use it */
    free(k);
}

void ky_buffer_grow(ky_state_t *k, size_t size) {
    if (!k->buffer) {
        k->m_buffer = size;
        k->buffer = ky_alloc(k, NULL, size); 
    } else if (k->m_buffer < size) {
        kmem growth = size - k->m_buffer;
        k->m_buffer += growth;
        k->m_total += growth;
        k->buffer = ky_alloc(k, k->buffer, size);
    }
}

void ky_buffer_free(ky_state_t *k) {
    k->m_total -= k->m_buffer;
    k->m_buffer = 0;
    kmem_free(k, k->buffer);
    k->buffer = NULL;
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