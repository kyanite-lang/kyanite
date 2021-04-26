#ifndef ky_string_h
#define ky_string_h

#include "kyobject.h"

/**
 * Represents a hashtable
 **/
typedef struct ky_htable_t {
    size_t        m_hash; /* size of hash table */
    size_t        n_hash; /* number of hashes in table */
    ky_string_t **hash;   /* pointer to hashtable */
} ky_htable_t;

void ky_htable_init(ky_state_t *k);
void ky_htable_resize(ky_state_t *k, size_t newsize);
void ky_htable_add(ky_state_t *k, ky_string_t *s, khash h);
void ky_htable_remove(ky_state_t *k, ky_string_t *s);

ky_string_t* ky_string_create(ky_state_t* k, const char *str);
ky_string_t* ky_string_find(ky_state_t* k, const char *str);
ky_string_t* ky_string_find_with_hash(ky_state_t* k, const char *str, khash hash);

#endif