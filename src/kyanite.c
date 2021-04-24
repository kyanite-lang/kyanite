#include "includes/kyanite.h"
#include "includes/kyparse.h"

#define FILE_MAX 1024*1024*1024

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

char* load_file(char *filename) {
    char *buffer;
    size_t file_length;
    size_t read_length;
    FILE *file = fopen(filename, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        file_length = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (file_length > FILE_MAX) {
            return NULL;
        }

        buffer = (char*)malloc(file_length + 1);
        if (file_length) {
            read_length = fread(buffer, sizeof(char), file_length, file);

            if (read_length != file_length) {
                fclose(file);
                free(buffer);
                return NULL;
            }
        }

        fclose(file);
        buffer[file_length] = '\0';
        return buffer;
    } else {
        return NULL;
    }
}

int do_kyanite(char *filename) {
    char *file = load_file(filename);
    if (file == NULL) {
        print_bad_output();
        printf("could not open that file\n");
        return -1;
    }
    ky_lexer_t lexer = ky_lexer(file);
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
    free(file);
    printf("\n");
    print_good_output();
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