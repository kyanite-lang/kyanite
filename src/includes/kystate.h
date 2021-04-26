#ifndef ky_state_h
#define ky_state_h

#include "kyanite.h"
#include "kyobject.h"
#include "kystring.h"

#define KY_MAX_ERR_FMT 256

typedef struct ky_state_t {
    kmem         m_total;  /* total used memory (excl. buff) */
    kbool        gc_on;    /* is GC running? */
    ky_object_t *o_all;    /* all objects */
    size_t       n_all;    /* number of objects */
    kstatus      status;   /* current status */
    char        *buffer;   /* general-use global buffer */
    size_t       m_buffer; /* size of buffer */
    ky_htable_t  htable;   /* hashtable for strings */
} ky_state_t;

ky_state_t* ky_state_new();
void ky_state_free(ky_state_t* k);
void ky_objects_free(ky_state_t* k);

#define KBUFF_GROW_SIZE(k) (((k->m_buffer / 2) > 16) ? (k->m_buffer / 2) : 16)

void ky_buffer_grow(ky_state_t *k, size_t size);
void ky_buffer_putch(ky_state_t *k, size_t index, char c);

void ky_error(ky_state_t* k, kstatus err, const char *str);
void ky_errorf(ky_state_t* k, kstatus err, const char *fmt, ...);

void ky_debug(ky_state_t* k, const char *str);
void ky_debugf(ky_state_t* k, const char *fmt, ...);

#endif