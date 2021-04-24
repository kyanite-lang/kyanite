#ifndef kylex_h
#define kylex_h

#include "kyanite.h"

#include <stddef.h>
#include <string.h>

typedef struct {
    char*  start;
    size_t length;
} ky_snippet_t;

char* ky_snippet_to_string(ky_snippet_t *snippet);

typedef enum {
    Function,           Object,          Event,              Invoke,
    On,                 Do,              End,                For,
    In,                 To,              By,                 If,
    Unless,             Then,            Else,               Break,
    Return,             Switch,          Typeswitch,         Default,
    LeftParen,          RightParen,      LeftList,           RightList,
    LeftVars,           RightVars,       Comma,              Dot,
    Colon,              Typename,        Is,                 Equal,
    EqualEqual,         Not,             NotEqual,           InverseOp,
    GreaterThan,        LessThan,        GreaterThanOrEqual, LessThanOrEqual, 
    Plus,               Minus,           Divide,             Multiply,
    PlusEqual,          MinusEqual,      DivideEqual,        MultiplyEqual,
    Modulo,             Power,           LeftShift,          RightShift,
    ModuloEqual,        PowerEqual,      LeftShiftEqual,     RightShiftEqual,
    PlusPlus,           MinusMinus,      Bang,               StringLiteral,
    IntLiteral,         FloatLiteral,    BooleanLiteral,     Identifier,
    Nil,                Newline,         EndOfStream,        Unknown
} ky_token_type_t;

typedef struct {
    ky_token_type_t type;
    ky_snippet_t    snippet;
} ky_token_t;

ky_token_t ky_token(ky_token_type_t type, char *start, size_t length);
ky_token_t ky_token_char(ky_token_type_t type, char *start, char *end);

typedef struct {
    char *stream;
} ky_lexer_t;

ky_lexer_t ky_lexer(char* stream);
ky_token_t ky_lexer_next(ky_lexer_t *l);
char ky_lexer_peek(ky_lexer_t *l);
char ky_lexer_get(ky_lexer_t *l);

ky_token_t ky_lexer_atom_get(ky_lexer_t *l, ky_token_type_t type);
ky_token_t ky_lexer_atom(ky_lexer_t *l, ky_token_type_t type);
ky_token_t ky_lexer_atom_at(ky_lexer_t *l, ky_token_type_t type, char *at);
ky_token_t ky_lexer_range_get(ky_lexer_t *l, ky_token_type_t type, char *start);
ky_token_t ky_lexer_range(ky_lexer_t *l, ky_token_type_t type, char *start);

kbool is_space(char c);
kbool is_alpha(char c);
kbool is_digit(char c);
kbool is_alpha_digit(char c);
kbool is_natural_digit(char c);
kbool is_special_alpha_digit(char c);
kbool is_newline(char c);

// TODO : parse-related functions

#endif