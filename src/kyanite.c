#include "includes/kyanite.h"
#include "includes/kylex.h"

#include <stdio.h>
#include <string.h>

char* _SAMPLE = "function potato(str)\n    print(str)\nend\n\nx = if p is __list then 1 else (x / (4 * 3))\n";

void print_badparams() {
    printf("%s\nusage: kyanite [-v]\n", KY_OUT_ERRPARAM);
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

int do_kyanite() {
    ky_lexer_t lexer = ky_lexer(_SAMPLE);
    for(ky_token_t t = ky_lexer_next(&lexer); t.type != EndOfStream; t = ky_lexer_next(&lexer)) {
        if (t.type == Unknown) {
            print_bad_output();
            printf("unknown token\n");
            return -1;
        }
        if (t.type == Newline) {
            printf("[\\n] ");
            continue;
        }
        char tok_str[t.snippet.length + 1];
        memset(tok_str, '\0', sizeof(tok_str));
        memcpy(tok_str, t.snippet.start, t.snippet.length);
        printf("`%s` ", tok_str);
    }
    printf("\n");
    print_good_output();
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        return do_kyanite();
    } else if (argc == 2) {
        if (strcmp(argv[1], "-v") == 0) {
            print_version();
            return 0;
        }
    }
    print_badparams();
    return -1;    
}