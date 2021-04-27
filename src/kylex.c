#include "includes/kylex.h"
#include "includes/kystate.h"
#include "includes/kystring.h"

#include <string.h>

#define knextch(p)     (p->current = kbs_getch(&p->kbs))
#define kswallow(p, t) knextch(p); return t
#define knextswitch(p) knextch(p); switch(p->current)

#define KINIT_BUFF_CHECK 128

/* token names - these must be in the order as defined in kylex.h */
static const char *const ktok_strings [] = {
    /* keywords */
    "function", "object", "event", "invoke", "on", "as", "call", "do", "end",
    "for", "in", "to", "by", "if", "unless", "then", "else", "break", "continue", 
    "return", "switch", "typeswitch", "default", "is", "not", "true",
    "false", "nil",
    /* operators & other */
    "==", ">=", "<=", "<<", ">>", "+=", "-=", "/=", "*=", "%=", "^=",
    "++", "--", "<eof>",
    "<integer>", "<float>", "<string>", "<type>", "<identifier>"
};

/* type names - these must be in the order as defined in kyanite.h */
static const char *const ktok_types[] = {
    "__int", "__float", "__bool", "__string", "__list"
};

static void ky_syntax_error(ky_parse_t *p, const char *msg) {
    ky_errorf(p->k, KY_ERR_SYN, "syntax error on line %d:\n%s", p->linenum, msg);
}

static void ky_syntax_errorf(ky_parse_t *p, const char *expect, char got) {
    ky_errorf(p->k, KY_ERR_SYN, "syntax error on line %d:\nexpecting %s, instead got %c", p->linenum, expect, got);
}

static ktt_t ky_parse_next_number(ky_parse_t *p, kts_u *sem) {
    if (!(_lex_is_digit(p->current))) {
        ky_syntax_errorf(p, "number", p->current);
    }
    ky_buffer_grow(p->k, KINIT_BUFF_CHECK);
    ky_syntax_error(p, "NUMBERS NOT YET IMPLEMENTED! SORRY!");
    /* TODO parse numbers */ KUNUSED(sem); return 0;
}

static ktt_t ky_parse_next_string(ky_parse_t *p, kts_u *sem) {
    if (p->current != '"') {
        ky_syntax_errorf(p, "string", p->current);
    }
    ky_buffer_grow(p->k, KINIT_BUFF_CHECK);
    knextch(p);
    size_t i = 0;
    while(p->current != '"') {
        if (p->current == KBS_EOS) {
            ky_syntax_error(p, "end of file reached inside string literal");
            break;
        }
        ky_buffer_putch(p->k, i++, p->current);
        knextch(p);
    }
    if (p->current != '"') {
        ky_syntax_errorf(p, "end of string", p->current);
    }
    ky_buffer_putch(p->k, i, '\0');
    ky_string_t *str = ky_string_create(p->k, p->k->buffer);
    sem->s = str;
    return tk_STRING;
}

static ktt_t ky_parse_next_type(ky_parse_t *p, kts_u *sem) {
    ky_buffer_grow(p->k, KINIT_BUFF_CHECK);
    size_t i = 0;
    if (p->current != '_') {
        ky_syntax_errorf(p, "type name", p->current);
    }
    ky_buffer_putch(p->k, i++, '_');
    knextch(p);
    if (p->current != '_') {
        ky_syntax_errorf(p, "type name", p->current);
    }
    ky_buffer_putch(p->k, i++, '_');
    knextch(p);
    while (_lex_is_alpha(p->current)) {
        ky_buffer_putch(p->k, i++, p->current);
        knextch(p);
    }
    if (i == 0) {
        ky_syntax_errorf(p, "type name", p->current);
    }
    ky_buffer_putch(p->k, i, '\0');
    ky_string_t* str = ky_string_find(p->k, p->k->buffer);
    if (str != NULL && _kstr_typename(str)) {
        sem->s = str;
        return tk_TYPE;
    } else {
        ky_syntax_error(p, "expected type name");
    }
    return 0;
}

static ktt_t ky_parse_next_ident(ky_parse_t *p, kts_u *sem) {
    if (!(_lex_is_alpha(p->current))) {
        ky_syntax_errorf(p, "identifier", p->current);
    }
    ky_buffer_grow(p->k, KINIT_BUFF_CHECK);
    size_t i = 0;
    do {
        ky_buffer_putch(p->k, i++, p->current);
        knextch(p);
    } while (_lex_is_special_alpha_digit(p->current));
    if (p->current == '?') {
        ky_buffer_putch(p->k, i++, '?');
        knextch(p);
    }
    ky_buffer_putch(p->k, i++, '\0');
    
    ky_string_t *str = ky_string_create(p->k, p->k->buffer);
    sem->s = str;
    if (_kstr_keyword(str)) {
        /* return the token corresponding to the keyword string's reserved byte */
        return FIRST_TOK_DEF + str->extra;
    }
    /* otherwise, return ident as normal */
    return tk_IDENT;
}

static void skip_newline(ky_parse_t *p) {
    knextch(p);
    ++p->linenum;
    /* TODO: catch too many lines */
}

void ky_parse_start(ky_parse_t *p, FILE *f) {
    kbs_fopen(&p->kbs, f);
    p->linenum = 1;
    for(int i = 0; i < NUM_TOK_KEYWORDS; i++) {
        ky_string_t *ks = ky_string_create(p->k, ktok_strings[i]);
        ks->special = KSTR_KEYWORD;
        ks->extra   = i;
    }
    for(int i = 0; i < KY_NUMCHECKABLE_TYPES; i++) {
        ky_string_t *ks = ky_string_create(p->k, ktok_types[i]);
        ks->special = KSTR_TYPENAME;
        ks->extra = KY_FIRST_CHECKABLE_TYPE + i;
    }
}

void ky_parse_finish(ky_parse_t *p) {
    if (p->kbs.status == KBS_GOOD)
        kbs_close(&p->kbs);
}

ktt_t ky_parse_next(ky_parse_t *p, kts_u *sem) {
    while (_lex_is_space(p->current)) knextch(p);

    if (p->current == KBS_EOS || p->current == '\0')
        return tk_EOS;

    if (_lex_is_newline(p->current)) {
        while(_lex_is_newline(p->current)) skip_newline(p);
        return '\n';
    }

    switch(p->current) {
        case '(': case ')': case '[': case ']':
        case '{': case '}': case ',': case '.':
        case ':': case '~':
            kswallow(p, p->current);
        case '+':
            knextswitch(p) {
                case '+': kswallow(p, tk_INC);
                case '=': kswallow(p, tk_ADDEQ);
                default:  return '+';
            }
        case '-':
            knextswitch(p) {
                case '-': kswallow(p, tk_DEC);
                case '=': kswallow(p, tk_MINEQ);
                default:  return '-';
            }
        case '/':
            knextswitch(p) {
                case '=': kswallow(p, tk_DIVEQ);
                default:  return '/';
            }
        case '*':
            knextswitch(p) {
                case '=': kswallow(p, tk_MULEQ);
                default:  return '*';
            }
        case '%':
            knextswitch(p) {
                case '=': kswallow(p, tk_MODEQ);
                default:  return '%';
            }
        case '^':
            knextswitch(p) {
                case '=': kswallow(p, tk_POWEQ);
                default:  return '^';
            }
        case '<':
            knextswitch(p) {
                case '<':
                    knextswitch(p) {
                        case '=': kswallow(p, tk_SHLEQ);
                        default:  return tk_SHL;
                    }
                case '=': kswallow(p, tk_LTE);
                default:  return '<';
            }
        case '>':
            knextswitch(p) {
                case '>':
                    knextswitch(p) {
                        case '=': kswallow(p, tk_SHREQ);
                        default:  return tk_SHR;
                    }
                case '=': kswallow(p, tk_GTE);
                default:  return '>';
            }
        case '=':
            knextswitch(p) {
                case '=': kswallow(p, tk_EQ);
                default:  return '=';
            }
        case '!':
            knextswitch(p) {
                case '=': kswallow(p, tk_NEQ);
                default:  return '!';
            }
        case '_':
            return ky_parse_next_type(p, sem);
        case '"':
            return ky_parse_next_string(p, sem);
    }

    if (_lex_is_digit(p->current)) return ky_parse_next_number(p, sem);
    if (_lex_is_alpha(p->current)) return ky_parse_next_ident(p, sem);

    ky_syntax_error(p, "unrecognized token");
    return 0;
}