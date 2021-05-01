#include "includes/kylex.h"
#include "includes/kyexec.h"

/**
 * Parsing
 **/

static void kp_err(ky_parse_t* p, const char *str) {
    ky_errorf(p->k, KY_ERR_SYN, "syntax error on line %d\n%s", p->linenum, str);
}
static void kp_err_unexpected(ky_parse_t* p, ktt_t got) {
    const char* got_str = ktok2str(got);
    if (got_str)
        ky_errorf(p->k, KY_ERR_SYN, "syntax error on line %d\nunexpected %s", p->linenum, got_str);
    else
        ky_errorf(p->k, KY_ERR_SYN, "syntax error on line %d\nunexpected %c", p->linenum, got);
}

static void kp_err_expected(ky_parse_t* p, ktt_t expect, ktt_t got) {
    const char* expect_str = ktok2str(expect);
    const char* got_str = ktok2str(got);
    if (expect_str && got_str)
        ky_errorf(p->k, KY_ERR_SYN, "syntax error on line %d\nexpected %s, got %s", p->linenum, expect_str, got_str);
    else if (expect_str)
        ky_errorf(p->k, KY_ERR_SYN, "syntax error on line %d\nexpected %s, got '%c'", p->linenum, expect_str, got);
    else if (got_str)
        ky_errorf(p->k, KY_ERR_SYN, "syntax error on line %d\nexpected '%c', got %s", p->linenum, expect, got_str);
    else
        ky_errorf(p->k, KY_ERR_SYN, "syntax error on line %d\nexpected '%c', got '%c'", p->linenum, expect, got);
}

static void kp_next(ky_parse_t* p) {
    p->tok = ky_lex(p, &(p->sem));
}

static void kp_require(ky_parse_t* p, ktt_t expect) {
    if (p->tok == expect)
        kp_next(p);
    else
        kp_err_expected(p, expect, p->tok);
}

static kbool kp_consume(ky_parse_t* p, ktt_t expect) {
    if (p->tok == expect) {
        kp_next(p);
        return KTRUE;
    }
    return KFALSE;
}

static ky_ident_t kp_ident(ky_parse_t* p, ky_scope_t* scope) {
    ky_string_t* s = p->sem.s;
    kp_require(p, tk_IDENT);
    return (ky_ident_t){ .idt = KY_IDENT_NAKED, .u.str = s };   
}

static ky_ident_t kp_full_ident(ky_parse_t* p, ky_scope_t* scope) {
    ky_string_t* first = p->sem.s;
    kp_require(p, tk_IDENT);
    ky_ident_node_t** previous = NULL;
    ky_ident_node_t* this = NULL;
    while (kp_consume(p, '.')) {
        ky_string_t* s = p->sem.s;
        kp_require(p, tk_IDENT);
        if (!this) {
            this = ky_ident_find_or_create(p->k, &(p->k->ids), first);
            previous = &(this->child);
        }
        this = ky_ident_find_or_create(p->k, previous, s);
        previous = &(this->child);
    }
    if (this)
        return (ky_ident_t){ .idt = KY_IDENT_QUALIFIED, .u.idn = this };
    return (ky_ident_t){ .idt = KY_IDENT_NAKED, .u.str = first };
}

static void kp_function_def(ky_parse_t* p, ky_scope_t* scope) {
    kp_require(p, tk_FUNCTION);
    ky_ident_t id1 = kp_ident(p, scope);
    if (kp_consume(p, '.')) {
        ky_ident_t id2 = kp_ident(p, scope);
        /* TODO : now check - is id1 an object? */
    } else if (kp_consume(p, ':')) {
        /* TODO : now check - is id1 an object? */
    }
}

static void kp_object_def(ky_parse_t* p, ky_scope_t* scope) {
    kp_require(p, tk_OBJECT);
}

static void kp_event_def(ky_parse_t* p, ky_scope_t* scope) {
    kp_require(p, tk_EVENT);
}

static ky_expression_t kp_expression(ky_parse_t* p, ky_scope_t* scope) {

}

static void kp_function_call(ky_parse_t* p, ky_scope_t* scope, ky_ident_t id) {
    kp_require(p, '(');
    
    if (p->tok != ')')
        kp_expression(p, scope);
}

static void kp_assignment(ky_parse_t* p, ky_scope_t* scope, ky_ident_t id) {
    ky_debug(p->k, "assignment has been called!");
}

static void kp_assignment_or_call(ky_parse_t* p, ky_scope_t* scope) {
    ky_ident_t id = kp_full_ident(p, scope);
    switch(p->tok) {
        case '(': kp_function_call(p, scope, id); break;
        case tk_ADDEQ: case tk_MINEQ:
        case tk_MULEQ: case tk_DIVEQ:
        case tk_SHLEQ: case tk_SHREQ:
        case tk_MODEQ: case tk_POWEQ:
        case tk_INC: case tk_DEC: case '=':
            kp_assignment(p, scope, id);
            break;
        default: kp_err(p, "expected function call or assignment");
    }
}

static void kp_flow(ky_parse_t* p, ky_scope_t* scope) {
    KUNUSED(p); KUNUSED(scope);
}

static void kp_statement(ky_parse_t* p, ky_scope_t* scope) {
    KUNUSED(p); KUNUSED(scope);
    while(kp_consume(p, '\n'));
    switch(p->tok) {
        case tk_FOR: kp_flow(p, scope); break;
        default:     kp_assignment_or_call(p, scope);
    }
}

static void kp_root_statement(ky_parse_t* p, ky_scope_t* scope) {
    switch (p->tok) {
        case tk_FUNCTION: kp_function_def(p, scope); break;
        case tk_OBJECT:   kp_object_def(p, scope); break;
        case tk_EVENT:    kp_event_def(p, scope); break;
        default:          kp_statement(p, scope);
    }
}

void ky_parse_all(ky_parse_t* p) {
    kp_next(p);
    while (p->tok != tk_EOS) {
        kp_root_statement(p, p->k->global);
    }
}