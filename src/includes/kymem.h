#ifndef ky_mem_h
#define ky_mem_h

#include "kystate.h"

void* ky_alloc(ky_state_t* k, void* ptr, size_t n);
void ky_mem_error(ky_state_t* k);

#endif