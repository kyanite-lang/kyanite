#include "includes/kyobject.h"
#include "includes/kymem.h"
#include "includes/kystring.h"

ky_object_t* ky_object_new(ky_state_t *k, kbyte type, size_t size) {
    size = (size > sizeof(ky_object_t)) ? size : sizeof(ky_object_t);
    ky_object_t *o = cast(ky_object_t*, ky_alloc(k, NULL, size));
    o->type = type;
    o->next = k->o_all;
    k->o_all = o;
    k->n_all++;
    k->m_total += size;
    return o;
}

void ky_object_free(ky_state_t *k, ky_object_t *o) {
    if (_is_string(o->type)) {
        ky_string_t *s = ko2str(o);
        ky_htable_remove(k, s);
    }
    // TODO: other object types
}