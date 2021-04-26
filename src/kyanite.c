#include "includes/kyanite.h"
#include "includes/kystate.h"
#include "includes/kylib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

kbool _print_version = KFALSE;

void print_badparams() {
    printf("%s\nusage: kyanite [-v|--version] <filename>\n", KY_OUT_ERRPARAM);
}

void print_version() {
    printf("%s\n", KY_VERSION_FULL);
}

void print_good_output() {
    printf("%s\n", KY_OUT_NOERR);
}

void print_bad_output() {
    printf("%s\n", KY_OUT_ERR);
}

int do_kyanite(char *filename) {
    ky_state_t *k = ky_state_new();
    _kyanite_read_file(k, filename);
    ky_debugf(k, "%ld/%ld hashtable entries. %ld objects. %ldB total memory.", k->htable.n_hash, k->htable.m_hash, k->n_all, k->m_total);
    ky_state_free(k);
    return 0;
}

int main(int argc, char *argv[]) {
    char *main_arg = NULL;
    if (argc > 1) {
        for(int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
                _print_version = KTRUE;
            } else {
                if (main_arg != NULL) {
                    print_badparams();
                    return -1;
                }
                main_arg = argv[i];
            }
        }
        if (_print_version) {
            print_version();
            return 0;
        } else if (main_arg != NULL) {
            return do_kyanite(main_arg);
        }
    }
    print_badparams();
    return -1;
}