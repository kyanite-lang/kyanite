#ifndef ky_mem_h
#define ky_mem_h

#include "kystate.h"

#define kmem_new(k, t) ((t *)ky_alloc(k, NULL, sizeof(t)))
#define kmem_free(k, p) ky_alloc(k, p, 0)

void* ky_alloc(ky_state_t* k, void* ptr, size_t n);
void ky_mem_error(ky_state_t* k);

#endif