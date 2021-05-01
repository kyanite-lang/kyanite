#ifndef ky_exec_h
#define ky_exec_h

#include "kyanite.h"
#include "kyobject.h"
#include "kylex.h"

#define KY_SCOPE_LOOSE  0 /* search in this scope and then parent scopes recursively */
#define KY_SCOPE_STRICT 1 /* only search in this scope */
#define KY_SCOPE_GLOBAL 2 /* traverse up to ONLY search in the root (global) scope */

/**
 * Represents a value within a table
 * of values
 **/
typedef struct ky_variable_t {
    ky_string_t   *ident; /* identifier/name of the variable */
    ky_value_t     value; /* value of the variable */
    struct ky_variable_t *next; /* next variable in the scope */
} ky_variable_t;

/**
 * Represents the unique data for each scope, i.e each instance
 * of a function being called (or the root program data if parent = NULL)
 **/
typedef struct ky_scope_t {
    struct ky_scope_t    *parent; /* enclosing scope; NULL if global (root scope) */
    kmem                  n_vars; /* number of variables in this scope */
    struct ky_variable_t *vars;   /* first variable in the list */
} ky_scope_t;

/**
 * Enum representing possible comparative operations
 **/
typedef enum {
    cmp_EQ, cmp_NEQ, cmp_GT, cmp_LT, cmp_GTE, cmp_LTE, cmp_IS
} ky_comp_t;

/* Forward declaration of expression for conditions */
typedef struct ky_expression_t ky_expression_t;

/**
 * Represents a language condition (construct that derives
 * a boolean value, usually based on variable inputs)
 **/
typedef struct ky_condition_t {
    ky_expression_t *left;  /* left of operator */
    ky_expression_t *right; /* right of operator */
    ky_comp_t        op;    /* operation */
} ky_condition_t;

/**
 * Possible types in a language expression
 **/
typedef union ky_expression_u {
    ky_string_t    *ident;     /* variable to resolve -> any */
    ky_condition_t *condition; /* condition to resolve -> bool */
    ky_value_t      value;     /* const value to resolve -> value.type */
} ky_expression_u;

#define KY_EXP_IDENT 1
#define KY_EXP_COND  2
#define KY_EXP_VALUE 3

/**
 * Represents a language expression (construct that can have value
 * derived from it)
 **/
typedef struct ky_expression_t {
    kbyte           type;
    ky_expression_u u;
} ky_expression_t;

/**
 * Represents the instruction of a method call
 **/
typedef struct ky_call_info_t {
    kmem              n_param;
    ky_expression_t  *param;
} ky_call_info_t;

ky_scope_t* ky_scope_new(ky_state_t* k, ky_scope_t* parent);
void ky_scope_free(ky_state_t* k, ky_scope_t* scope);
void ky_scope_variable_add(ky_state_t* k, ky_scope_t* scope, ky_string_t* ident, ky_value_t value);
ky_variable_t* ky_scope_variable_find(ky_state_t* k, ky_scope_t* scope, ky_string_t* ident, kbyte search_type);

ky_call_info_t* ky_call_new(ky_state_t* k, ky_scope_t* scope, ky_method_t* method);
void ky_call_return(ky_state_t* k, ky_call_info_t* c, ky_value_t* ret);

#endif