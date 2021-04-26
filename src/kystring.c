#include "includes/kystring.h"
#include "includes/kymem.h"

#include <string.h>

/* index a hash should go in the table based on the max size of the table */
#define khash2index(h, s) (h & (s-1))

/**
 * djb2 hash algorithm
 **/
static khash khash_djb2(const char *str) {
    khash h = 5381;
    int c;
    while ((c = *str++)) {
        h = ((h << 5) + h) + c;
    }
    return h;
}

void ky_htable_init(ky_state_t *k) {
    if (!k->htable.hash) {
        k->htable.hash = cast(ky_string_t**, ky_alloc(k, NULL, sizeof(ky_string_t*)));
        k->htable.hash[0] = NULL;
        k->htable.m_hash = 1;
        k->htable.n_hash = 0;
        k->m_total += sizeof(ky_string_t*);
    }
}

void ky_htable_resize(ky_state_t *k, size_t newsize) {
    ky_string_t **newhtable = cast(ky_string_t**, ky_alloc(k, NULL, newsize * sizeof(ky_string_t*)));
    size_t i;
    for (i = 0; i < newsize; i++) newhtable[i] = NULL;
    for (i = 0; i < k->htable.n_hash; i++) {
        ky_string_t *s = k->htable.hash[i];
        size_t newhi = khash2index(s->hash, newsize);
        newhtable[newhi] = s;
    }
    ky_alloc(k, k->htable.hash, 0);
    k->m_total += (newsize - k->htable.m_hash);
    k->htable.m_hash = newsize;
    k->htable.hash = newhtable;
}

void ky_htable_add(ky_state_t* k, ky_string_t* s, khash hash) {
    size_t hi = khash2index(hash, k->htable.m_hash);
    if (k->htable.hash[hi]) {
        s->chain = k->htable.hash[hi];
        k->htable.hash[hi] = s;
    } else {
        k->htable.hash[hi] = s;
        k->htable.n_hash++;
        if (k->htable.n_hash >= k->htable.m_hash)
            ky_htable_resize(k, k->htable.m_hash * 2);
    }
}

void ky_htable_remove(ky_state_t *k, ky_string_t *s) {
    KUNUSED(s);
    ky_error(k, KY_ERR_MEM, "warning! i've not implemented ky_htable_remove yet teehee");
    /*int hi = khash2index(s->hash, k->htable.m_hash);
    if (k->htable.hash[hi]) {
        k->htable.hash[hi] = NULL;
        k->htable.n_hash--;
    } else {
        ky_error(k, KY_ERR_MEM, "tried to delete string that did not exist in hashtable");
    }*/
}

ky_string_t* ky_string_create(ky_state_t* k, const char *str) {
    /* get length and hash of this string */
    size_t length = strlen(str);
    khash h = khash_djb2(str);
    /* return ky_string_t* for the string if already in hashtable */
    ky_string_t *s;
    if ((s = ky_string_find_with_hash(k, str, h))) {
        return s;
    }
    /* get the total size needed for a ky_string_t of this length */
    size_t total = ksize_str(length);
    /* create and register the object and get its string value */
    ky_object_t *o = ky_object_new(k, KY_TYPE_STRING, total);
    ky_string_t *ns = ko2str(o);
    /* copy the string contents into the string and null-terminate it */
    memcpy(kstr(ns), str, length);
    kstr(ns)[length] = '\0';
    ns->size = length;
    ns->hash = h;
    ns->chain = NULL;
    ns->special = 0; /* special byte is 0 (default, GC-collectable) for now */
    ky_htable_add(k, ns, h);
    return ns;
}

ky_string_t* ky_string_find_with_hash(ky_state_t* k, const char *str, khash hash) {
    /* get length of string (not including final \0) */
    size_t length = strlen(str);
    int hi = khash2index(hash, k->htable.m_hash);
    ky_string_t *s = k->htable.hash[hi];
    while (s) {
        /* this is promising - check for a match */
        if (s->size == length && memcmp(str, s->cs, length) == 0) {
            /* match ! return it */
            return s;
        }
        /* keep looking through the hash chain */
        s = s->chain;
    }
    /* no match, return NULL for not found */
    return NULL;
}

ky_string_t* ky_string_find(ky_state_t* k, const char *str) {
    khash h = khash_djb2(str);
    return ky_string_find_with_hash(k, str, h);
}