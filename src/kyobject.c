#include "includes/kyobject.h"
#include "includes/kymem.h"
#include "includes/kystring.h"

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