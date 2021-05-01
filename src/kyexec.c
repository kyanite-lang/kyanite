#include "includes/kyexec.h"
#include "includes/kymem.h"

/**
 * Variables & scopes
 **/

ky_scope_t* ky_scope_new(ky_state_t* k, ky_scope_t* parent) {
    ky_scope_t *scope = kmem_new(k, ky_scope_t);
    k->m_total += sizeof(ky_scope_t);
    scope->parent = parent;
    scope->n_vars = 0;
    scope->vars = NULL;
    return scope;
}

void ky_scope_free(ky_state_t* k, ky_scope_t* scope) {
    ky_variable_t *var = scope->vars;
    while (var) {
        ky_variable_t *next = var->next;
        k->m_total -= sizeof(ky_variable_t);
        kmem_free(k, var);
        var = next;
    }
    k->m_total -= sizeof(ky_scope_t);
    kmem_free(k, scope);
}

void ky_scope_variable_add(ky_state_t* k, ky_scope_t* scope, ky_string_t* ident, ky_value_t value) {
    ky_variable_t* v = kmem_new(k, ky_variable_t);
    k->m_total += sizeof(ky_variable_t);
    v->ident = ident;
    v->value = value;
    scope->n_vars++;
    v->next = scope->vars;
    scope->vars = v;
}

ky_variable_t* ky_scope_variable_find(ky_state_t* k, ky_scope_t* scope, ky_string_t* ident, kbyte search_type) {
    if (ident == NULL)
        return NULL;
    if (search_type == KY_SCOPE_GLOBAL && scope->parent != NULL)
        return ky_scope_variable_find(k, scope->parent, ident, search_type);
    ky_variable_t* v = scope->vars;
    while (v) {
        if (v->ident->hash == ident->hash)
            return v;
        v = v->next;
    }
    if (search_type == KY_SCOPE_LOOSE && scope->parent != NULL)
        return ky_scope_variable_find(k, scope->parent, ident, search_type);
    return NULL;
}

/**
 * Function calls
 **/

ky_call_info_t* ky_call_new(ky_state_t* k, ky_scope_t* scope, ky_method_t* method) {
    KUNUSED(k); KUNUSED(scope); KUNUSED(method); return NULL;
}

void ky_call_return(ky_state_t* k, ky_call_info_t* c, ky_value_t* ret) {
    KUNUSED(k); KUNUSED(c); KUNUSED(ret);
}