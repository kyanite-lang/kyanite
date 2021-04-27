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

/**
 * initiate the hashtable with space for 1 hash
 **/
void ky_htable_init(ky_state_t *k) {
    if (!k->htable.hash) {
        k->htable.hash = cast(ky_string_t**, ky_alloc(k, NULL, sizeof(ky_string_t*)));
        k->htable.hash[0] = NULL;
        k->htable.m_hash = 1;
        k->htable.n_hash = 0;
        k->m_total += sizeof(ky_string_t*);
    }
}

/**
 * free the hashtable
 **/
void ky_htable_free(ky_state_t *k) {
    if (k->htable.n_hash > 0)
        ky_errorf(k, KY_ERR_MEM, "tried to free hashtable which still contained %ld strings", k->htable.n_hash);
    kmem_free(k, k->htable.hash);
    k->htable.hash = NULL;
    k->htable.m_hash = 0;
}

/**
 * shrink if necessary
 **/
void ky_htable_shrink(ky_state_t *k) {
    if (k->htable.n_hash < (k->htable.m_hash / 4) && k->htable.m_hash > 16) {
        ky_htable_resize(k, (k->htable.m_hash / 2));
    }
}

/**
 * resize the table and put hashes in their new places
 **/
void ky_htable_resize(ky_state_t *k, size_t newsize) {
    ky_string_t **newhtable = cast(ky_string_t**, ky_alloc(k, NULL, newsize * sizeof(ky_string_t*)));
    size_t i;
    for (i = 0; i < newsize; i++) newhtable[i] = NULL;
    for (i = 0; i < k->htable.n_hash; i++) {
        ky_string_t *s = k->htable.hash[i];
        while (s) {
            ky_string_t *oldchain = s->chain;
            size_t newhi = khash2index(s->hash, newsize);
            s->chain = newhtable[newhi];
            newhtable[newhi] = s;
            s = oldchain;
        }
    }
    kmem_free(k, k->htable.hash);
    k->m_total += (newsize - k->htable.m_hash);
    k->htable.m_hash = newsize;
    k->htable.hash = newhtable;
}

/**
 * add a string to the hashtable
 **/
void ky_htable_add(ky_state_t* k, ky_string_t* s) {
    kmem hi = khash2index(s->hash, k->htable.m_hash);
    if (k->htable.hash[hi]) {
        s->chain = k->htable.hash[hi];
        k->htable.hash[hi] = s;
    } else {
        k->htable.hash[hi] = s;
    }
    k->htable.n_hash++;
    if (k->htable.n_hash >= k->htable.m_hash)
        ky_htable_resize(k, k->htable.m_hash * 2);
}

/**
 * Remove a hash from the hashtable
 **/
void ky_htable_remove(ky_state_t *k, ky_string_t *s) {
    kmem hi = khash2index(s->hash, k->htable.m_hash);
    ky_string_t **prev = &(k->htable.hash[hi]);
    while (*prev != s)
        prev = &((*prev)->chain);
    *prev = (*prev)->chain;
    k->htable.n_hash--;
}

/**
 * create a string from a const char* and add to
 * the hashtable
 **/
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
    ky_htable_add(k, ns);
    return ns;
}

/**
 * find a string by its contents and a hash of the contents
 * (use ky_string_find if the hash hasn't already been calculated)
 **/
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

/**
 * find a string by its contents
 * (use ky_string_find_with_hash if you already know the hash to save computing again)
 **/
ky_string_t* ky_string_find(ky_state_t* k, const char *str) {
    khash h = khash_djb2(str);
    return ky_string_find_with_hash(k, str, h);
}