#include "includes/kystream.h"

void kbs_fopen(kbs_t *kbs, FILE *f) {
    if (f == NULL) return;
    kbs->n = 0;
    kbs->p = kbs->buff;
    kbs->f = f;
    kbs->status = KBS_GOOD;
}

char kbs_read(kbs_t *kbs) {
    if (kbs->status != KBS_GOOD)
        return KBS_EOS;
    size_t n;
    if (feof(kbs->f)) {
        fclose(kbs->f);
        kbs->status = KBS_CLOSED;
        return KBS_EOS;
    }
    n = fread(kbs->buff, 1, KBS_SIZE, kbs->f);
    if (n == 0) {
        fclose(kbs->f);
        kbs->status = KBS_CLOSED;
        return KBS_EOS;
    }
    kbs->n = n-1;
    kbs->p = kbs->buff;
    return *(kbs->p++);
}

char kbs_getch(kbs_t *kbs) {
    if (kbs->n-- > 0) {
        return (char) *kbs->p++;
    }
    return kbs_read(kbs);
}

void kbs_ungetch(kbs_t *kbs) {
    ++kbs->n;
    --kbs->p;
}

void kbs_close(kbs_t *kbs) {
    if (kbs->status != KBS_GOOD) return;
    fclose(kbs->f);
    kbs->status = KBS_CLOSED;
}