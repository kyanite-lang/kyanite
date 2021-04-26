#ifndef ky_stream_h
#define ky_stream_h

#include <stddef.h>
#include <stdio.h>

#define KBS_EOS  (-1)
#define KBS_SIZE 256

typedef unsigned short kbs_status_t;
#define KBS_NIL    0
#define KBS_GOOD   1
#define KBS_BAD    2
#define KBS_CLOSED 3

/**
 * kyanite buffered stream
 **/
typedef struct {
    unsigned char *p;              /* current position in buffer */
    size_t         n;              /* bytes left on buffer */
    unsigned char  buff[KBS_SIZE]; /* buffer */
    FILE          *f;              /* pointer to source file */
    kbs_status_t   status;         /* status of source file */
} kbs_t;

void kbs_fopen(kbs_t *kbs, FILE *f);
char kbs_read(kbs_t *kbs);
char kbs_getch(kbs_t *kbs);
void kbs_ungetch(kbs_t *kbs);
void kbs_close(kbs_t *kbs);

#endif