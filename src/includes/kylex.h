#ifndef kylex_h
#define kylex_h

#include "kyanite.h"
#include "kystate.h"
#include "kystream.h"

/* maximum characters to print of source in case of syntax err */
#define MAX_ERR_SYN_SRC 48

/**
 * short-hand for token type
 * <= UCHAR_MAX, is identical to the token as an unsigned char,
 * >  UCHAR_MAX, matches one of the pre-defined symbols
 **/
typedef unsigned short ktt_t;

/**
 * represents the semantic information a token can hold
 **/
typedef union {
    kfloat       f;
    kint         i;
    ky_string_t *s;
} kts_u;

/**
 * represents a token and any semantic information
 **/
typedef struct {
    ktt_t type; /* token type */
    kts_u sem;  /* semantic information */
} ktok_t;

/**
 * first token definition, less than which are only
 * single-char tokens representable in and of themselves
 * as characters
 **/
#define FIRST_TOK_DEF (UCHAR_MAX + 1)

/* other tokens */
enum TOK_DEF {
    /* start with keywords */
    tk_FUNCTION = FIRST_TOK_DEF, tk_OBJECT, tk_EVENT, tk_INVOKE, tk_ON, tk_AS,
    tk_CALL, tk_DO, tk_END, tk_FOR, tk_IN, tk_TO, tk_BY, tk_IF, tk_UNLESS, tk_THEN,
    tk_ELSE, tk_BREAK, tk_CONTINUE, tk_RETURN, tk_SWITCH, tk_TYPESWITCH, tk_DEFAULT,
    tk_IS, tk_NOT, tk_TRUE, tk_FALSE, tk_NIL,
    /* then go on to symbols */
    tk_EQ, tk_NEQ, tk_GTE, tk_LTE, tk_SHL, tk_SHR, tk_ADDEQ, tk_MINEQ, tk_DIVEQ,
    tk_MULEQ, tk_MODEQ, tk_POWEQ, tk_SHLEQ, tk_SHREQ, tk_INC, tk_DEC, tk_EOS,
    tk_INT, tk_FLOAT, tk_STRING, tk_TYPE, tk_IDENT
};

/* highest token index */
#define LAST_TOK_DEF ((cast(int, (tk_IDENT - FIRST_TOK_DEF))))
/* number of token keywords */
#define NUM_TOK_KEYWORDS (cast(int, (tk_NIL - FIRST_TOK_DEF + 1)))

/**
 * holds state of lex/parse
 **/
typedef struct ky_parse_t {
    ky_state_t *k;       /* main kyanite state */
    char        current; /* current character */
    int         linenum; /* current line number */
    ktt_t       tok;     /* current token */
    kts_u       sem;     /* current semantic informatino */
    kbs_t       kbs;     /* input stream */
} ky_parse_t;

/**
 * kylex.c
 **/
void ky_parse_init(ky_parse_t *p, FILE *f);
ktt_t ky_lex(ky_parse_t *p, kts_u *sem);

/**
 * kyparse.c
 **/
void ky_parse_all(ky_parse_t* p);

const char *ktok2str(ktt_t tok);

/**
 * useful builtins for lexing
 **/
#define _lex_is_space(c) (c == ' ' || c == '\t')
#define _lex_is_alpha(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define _lex_is_digit(c) (c >= '1' && c <= '0')
#define _lex_is_alpha_digit(c) (_lex_is_alpha(c) || _lex_is_digit(c))
#define _lex_is_natural_digit(c) (c >= '1' && c <= '9')
#define _lex_is_special_alpha_digit(c) (_lex_is_alpha_digit(c) || c == '_')
#define _lex_is_newline(c) (c == '\n' || c == '\r')

#endif