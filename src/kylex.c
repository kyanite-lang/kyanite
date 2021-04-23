#include "includes/kylex.h"

/**
 * ky_token_t functions
 **/
ky_token_t ky_token(ky_token_type_t type, char *start, size_t length) {
    return (ky_token_t) { type, { start, length } };
}

ky_token_t ky_token_char(ky_token_type_t type, char *start, char *end) {
    return (ky_token_t) { type, { start, (end-start) } };
}

/**
 * ky_lexer_t functions
 **/
ky_lexer_t ky_lexer(char* stream) {
    return (ky_lexer_t){ stream };
}

char ky_lexer_peek(ky_lexer_t *l) {
    return *(l->stream);
}

char ky_lexer_get(ky_lexer_t *l) {
    return *(l->stream++);
}

/**
 * ky_lexer_t token-builders
 **/
ky_token_t ky_lexer_atom_get(ky_lexer_t *l, ky_token_type_t type) {
    return ky_token(type, l->stream++, 1);
}
ky_token_t ky_lexer_atom(ky_lexer_t *l, ky_token_type_t type) {
    return ky_token(type, l->stream, 1);
}
ky_token_t ky_lexer_atom_at(ky_lexer_t *l, ky_token_type_t type, char *at) {
    return ky_token(type, at, 1);
}
ky_token_t ky_lexer_string_get(ky_lexer_t *l, ky_token_type_t type, char *start) {
    return ky_token_char(type, start, l->stream++);
}
ky_token_t ky_lexer_string(ky_lexer_t *l, ky_token_type_t type, char *start) {
    return ky_token_char(type, start, l->stream);
}

/**
 * useful builtins
 **/
int is_space(char c) {
    if (c == ' ' || c == '\t') return 1;
    return 0;
}
int is_alpha(char c) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) return 1;
    return 0;
}
int is_digit(char c) {
    if (c >= 48 && c <= 57) return 1;
    return 0;
}
int is_alpha_digit(char c) {
    if (is_digit(c) || is_alpha(c)) return 1;
    return 0;
}
int is_natural_digit(char c) {
    if (c >= 49 && c <= 57) return 1;
    return 0;
}
int is_special_alpha_digit(char c) {
    if (is_alpha_digit(c) || c == '_') return 1;
    return 0;
}
int is_newline(char c) {
    if (c == '\n' || c == '\r') return 1;
    return 0;
}

/**
 * the actual parse!
 **/
ky_token_t ky_lexer_next(ky_lexer_t *l) {
    while (is_space(ky_lexer_peek(l))) ky_lexer_get(l);

    if (ky_lexer_peek(l) == '\0') return ky_lexer_atom(l, EndOfStream);

    char *start = l->stream;

    if (is_newline(ky_lexer_peek(l))) {
        while (is_newline(ky_lexer_peek(l))) ky_lexer_get(l);
        return ky_lexer_atom(l, Newline);
    }

    switch (ky_lexer_peek(l)) {
        case '(': return ky_lexer_atom_get(l, LeftParen);
        case ')': return ky_lexer_atom_get(l, RightParen);
        case '[': return ky_lexer_atom_get(l, LeftList);
        case ']': return ky_lexer_atom_get(l, RightList);
        case '{': return ky_lexer_atom_get(l, LeftVars);
        case '}': return ky_lexer_atom_get(l, RightVars);
        case ',': return ky_lexer_atom_get(l, Comma);
        case '.': return ky_lexer_atom_get(l, Dot);
        case ':': return ky_lexer_atom_get(l, Colon);
        case '~': return ky_lexer_atom_get(l, InverseOp);
        case '+':
            ky_lexer_get(l);
            switch (ky_lexer_peek(l)) {
                case '+': return ky_lexer_string_get(l, PlusPlus, start);
                case '=': return ky_lexer_string_get(l, PlusEqual, start);
            }
            return ky_lexer_atom(l, Plus);
        case '-':
            ky_lexer_get(l);
            switch (ky_lexer_peek(l)) {
                case '-': return ky_lexer_string_get(l, MinusMinus, start);
                case '=': return ky_lexer_string_get(l, MinusEqual, start);
            }
            return ky_lexer_atom_at(l, Minus, start);
        case '/':
            ky_lexer_get(l);
            if (ky_lexer_peek(l) == '=') return ky_lexer_string_get(l, DivideEqual, start);
            return ky_lexer_atom_at(l, Divide, start);
        case '*':
            ky_lexer_get(l);
            if (ky_lexer_peek(l) == '=') return ky_lexer_string_get(l, MultiplyEqual, start);
            return ky_lexer_atom_at(l, Multiply, start);
        case '%':
            ky_lexer_get(l);
            if (ky_lexer_peek(l) == '=') return ky_lexer_string_get(l, ModuloEqual, start);
            return ky_lexer_atom_at(l, Modulo, start);
        case '^':
            ky_lexer_get(l);
            if (ky_lexer_peek(l) == '=') return ky_lexer_string_get(l, PowerEqual, start);
            return ky_lexer_atom_at(l, Power, start);
        case '<':
            ky_lexer_get(l);
            switch (ky_lexer_peek(l)) {
                case '<':
                    ky_lexer_get(l);
                    if (ky_lexer_peek(l) == '=') return ky_lexer_string_get(l, LeftShiftEqual, start);
                    return ky_lexer_atom_at(l, LeftShift, start);
                case '=': return ky_lexer_string_get(l, LessThanOrEqual, start);
            }
            return ky_lexer_atom_at(l, LessThan, start);
        case '>':
            ky_lexer_get(l);
            switch (ky_lexer_peek(l)) {
                case '>':
                    ky_lexer_get(l);
                    if (ky_lexer_peek(l) == '=') return ky_lexer_string_get(l, RightShiftEqual, start);
                    return ky_lexer_atom_at(l, RightShift, start);
                case '=': return ky_lexer_string_get(l, GreaterThanOrEqual, start);
            }
            return ky_lexer_atom_at(l, GreaterThan, start);
        case '=':
            ky_lexer_get(l);
            if (ky_lexer_peek(l) == '=') return ky_lexer_string_get(l, EqualEqual, start);
            return ky_lexer_atom_at(l, Equal, start);
        case '!':
            ky_lexer_get(l);
            if (ky_lexer_peek(l) == '=') return ky_lexer_string_get(l, NotEqual, start);
            return ky_lexer_atom_at(l, Bang, start);
        case '_':
            ky_lexer_get(l);
            if (ky_lexer_peek(l) == '_') {
                ky_lexer_get(l);
                while (is_alpha(ky_lexer_peek(l))) ky_lexer_get(l);
                size_t typestring_length = l->stream - start;
                char typestring[typestring_length + 1];
                memset(typestring, '\0', sizeof(typestring));
                memcpy(typestring, start, typestring_length);
                if (strcmp(typestring, "__list") == 0 || strcmp(typestring, "__int") == 0 ||
                    strcmp(typestring, "__bool") == 0 || strcmp(typestring, "__float") == 0 ||
                    strcmp(typestring, "__string") == 0) {
                    return ky_lexer_string(l, Typename, start);
                }
            }
            return ky_lexer_string_get(l, Unknown, start);
    }

    if (ky_lexer_peek(l) == '"') {
        ky_lexer_get(l);
        while(ky_lexer_peek(l) != '"' && ky_lexer_peek(l) != '\0') {
            ky_lexer_get(l);
        }
        if (ky_lexer_peek(l) == '"') return ky_lexer_string_get(l, StringLiteral, start);
        return ky_lexer_string(l, Unknown, start);
    }

    if (ky_lexer_peek(l) == '0') {
        ky_lexer_get(l);
        if (ky_lexer_peek(l) != '.') return ky_lexer_atom_at(l, IntLiteral, start);
        ky_lexer_get(l);
        if (!is_digit(ky_lexer_peek(l))) return ky_lexer_string(l, Unknown, start);
        while(is_digit(ky_lexer_peek(l))) ky_lexer_get(l);
        return ky_lexer_string(l, FloatLiteral, start);
    }

    if (is_digit(ky_lexer_peek(l))) {
        ky_lexer_get(l);
        if (ky_lexer_peek(l) == '.') {
            ky_lexer_get(l);
            if (!is_digit(ky_lexer_peek(l))) return ky_lexer_string(l, Unknown, start);
            while(is_digit(ky_lexer_peek(l))) ky_lexer_get(l);
            return ky_lexer_string(l, FloatLiteral, start);
        }
        if (is_digit(ky_lexer_peek(l))) {
            while (is_digit(ky_lexer_peek(l))) ky_lexer_get(l);
        }
        return ky_lexer_string(l, IntLiteral, start);
    }

    if (is_alpha(ky_lexer_peek(l))) {
        ky_lexer_get(l);
        while(is_special_alpha_digit(ky_lexer_peek(l))) ky_lexer_get(l);
        if (ky_lexer_peek(l) == '?') ky_lexer_get(l);
        size_t ident_length = l->stream - start;
        char ident[ident_length + 1];
        memset(ident, '\0', sizeof(ident));
        memcpy(ident, start, ident_length);
        if (strcmp(ident, "function") == 0)   return ky_lexer_string(l, Function, start);
        if (strcmp(ident, "object") == 0)     return ky_lexer_string(l, Object, start);
        if (strcmp(ident, "event") == 0)      return ky_lexer_string(l, Event, start);
        if (strcmp(ident, "invoke") == 0)     return ky_lexer_string(l, Invoke, start);
        if (strcmp(ident, "on") == 0)         return ky_lexer_string(l, On, start);
        if (strcmp(ident, "do") == 0)         return ky_lexer_string(l, Do, start);
        if (strcmp(ident, "end") == 0)        return ky_lexer_string(l, End, start);
        if (strcmp(ident, "for") == 0)        return ky_lexer_string(l, For, start);
        if (strcmp(ident, "in") == 0)         return ky_lexer_string(l, In, start);
        if (strcmp(ident, "to") == 0)         return ky_lexer_string(l, To, start);
        if (strcmp(ident, "by") == 0)         return ky_lexer_string(l, By, start);
        if (strcmp(ident, "if") == 0)         return ky_lexer_string(l, If, start);
        if (strcmp(ident, "unless") == 0)     return ky_lexer_string(l, Unless, start);
        if (strcmp(ident, "then") == 0)       return ky_lexer_string(l, Then, start);
        if (strcmp(ident, "else") == 0)       return ky_lexer_string(l, Else, start);
        if (strcmp(ident, "break") == 0)      return ky_lexer_string(l, Break, start);
        if (strcmp(ident, "return") == 0)     return ky_lexer_string(l, Return, start);
        if (strcmp(ident, "switch") == 0)     return ky_lexer_string(l, Switch, start);
        if (strcmp(ident, "typeswitch") == 0) return ky_lexer_string(l, Typeswitch, start);
        if (strcmp(ident, "default") == 0)    return ky_lexer_string(l, Default, start);
        if (strcmp(ident, "nil") == 0)        return ky_lexer_string(l, Nil, start);
        if (strcmp(ident, "true") == 0 || strcmp(ident, "false") == 0) return ky_lexer_string(l, BooleanLiteral, start);
        return ky_lexer_string(l, Identifier, start);
    }

    return ky_lexer_atom_get(l, Unknown);
}