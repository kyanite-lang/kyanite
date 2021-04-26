#ifndef ky_lib_h
#define ky_lib_h

#include "kyanite.h"

void _kyanite_require_version(ky_state_t *k, kint version);

void _kyanite_read_file(ky_state_t *k, const char *filename);

#endif