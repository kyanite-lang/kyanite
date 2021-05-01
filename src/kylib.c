#include "includes/kylib.h"
#include "includes/kylex.h"

#include <stdio.h>

void _kyanite_require_version(ky_state_t *k, kint version) {
    if (version != KY_VERSION_NUM) {
        ky_errorf(k, KY_ERR_RUN, "additional library wants Kyanite ver %d but this is ver %d", version, KY_VERSION_NUM);
    }
}

void _kyanite_read_file(ky_state_t *k, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        ky_errorf(k, KY_ERR_RUN, "file %s could not be opened!", filename);
        return;
    }
    ky_parse_t p;
    p.k = k;
    ky_parse_init(&p, f);
    ky_parse_all(&p);
}