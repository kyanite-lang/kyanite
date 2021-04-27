#ifndef ky_gc_h
#define ky_gc_h

#include "kyanite.h"

/**
 * levels of GC collection
 **/
#define KY_GC_COLLECTION_STRICT 0
#define KY_GC_COLLECTION_ALL    1

void ky_gc_collect(ky_state_t* k, kbyte collection);
void ky_gc_check(ky_state_t* k);

#endif