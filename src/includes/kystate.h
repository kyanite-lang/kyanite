#ifndef ky_state_h
#define ky_state_h

#include "kyanite.h"
#include "kyobject.h"
#include "kystring.h"
#include "kyexec.h"

#define KBUFF_GROW_SIZE(k) (((k->m_buffer * 2) > 16) ? (k->m_buffer * 2) : 16)

#define KY_MAX_ERR_FMT 256

typedef struct ky_state_t {
    kmem            m_total;   /* total used memory */
    kbool            gc_on;     /* is GC running? */
    kmem             gc_thresh; /* mem threshold for running GC */
    kmem             m_gco;     /* total memory of all GC-able objects */
    kmem             n_gco;     /* number of GC-able objects */
    ky_object_t     *gco;       /* all GC-able objects */
    ky_ident_node_t *ids;       /* first node in qualified ident tree */
    kmem             m_buffer;  /* size of buffer */
    char            *buffer;    /* general-use global buffer */
    ky_htable_t      htable;    /* hashtable for strings */
    ky_scope_t      *global;    /* the global scope (vars/methods) */
    ky_method_t     *code;      /* root program instructions */
    kstatus          status;    /* current status */
} ky_state_t;

ky_state_t* ky_state_new();
void ky_state_close(ky_state_t* k);

void ky_buffer_grow(ky_state_t *k, size_t size);
void ky_buffer_free(ky_state_t *k);
void ky_buffer_putch(ky_state_t *k, size_t index, char c);

void ky_error(ky_state_t* k, kstatus err, const char *str);
void ky_errorf(ky_state_t* k, kstatus err, const char *fmt, ...);

void ky_debug(ky_state_t* k, const char *str);
void ky_debugf(ky_state_t* k, const char *fmt, ...);

#endif