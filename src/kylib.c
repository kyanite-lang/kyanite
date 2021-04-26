#include "includes/kylib.h"
#include "includes/kylex.h"

#include <stdio.h>

void _kyanite_require_version(ky_state_t *k, kint version) {
    if (version != KY_VERSION_NUM) {
        ky_errorf(k, KY_ERR_RUN, "additional library wants Kyanite ver %d but this is ver %d", version, KY_VERSION_NUM);
    }
}

void _kyanite_read_file(ky_state_t *k, const char *filename) {
    KUNUSED(k);
    FILE *f = fopen(filename, "r");
    ky_parse_t p;
    p.k = k;
    ky_parse_start(&p, f);
}