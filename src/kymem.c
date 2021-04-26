#include "includes/kymem.h"

#include <stdlib.h>

/**
 * free, newly allocate or reallocate space on the heap
 **/
void* ky_alloc(ky_state_t* k, void* ptr, size_t n) {
    if (n <= 0) {
        if (ptr != NULL)
            free(ptr);
        return NULL;
    }
    if (ptr == NULL) {
        ptr = malloc(n);
    } else {
        ptr = realloc(ptr, n);
    }
    if (ptr == NULL)
        ky_mem_error(k);
    return ptr;
}

void ky_mem_error(ky_state_t* k) {
    ky_error(k, KY_ERR_MEM, "memory allocation error");
}