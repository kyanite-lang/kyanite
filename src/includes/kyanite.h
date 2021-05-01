#ifndef kyanite_h
#define kyanite_h

#include <limits.h> /* MAX_% macros */
#include <stddef.h> /* size_t, ptrdiff_t */

/* forward declaration of ky_state_t (see kystate.h) */
typedef struct ky_state_t ky_state_t;

/* forward declaration of ky_scope_t (see kyexec.h) */
typedef struct ky_scope_t ky_scope_t;

/* forward declaration of ky_call_info_t (see kyexec.h) */
typedef struct ky_call_info_t ky_call_info_t;

/* forward declaration of ky_parse_t (see kylex.h) */
typedef struct ky_parse_t ky_parse_t;

/* version */
#define KY_VERSION_MAJOR "0"
#define KY_VERSION_MINOR "0"
#define KY_VERSION_PATCH "1"
#define KY_VERSION_STAGE "alpha"

#define KY_VERSION_NUM    1
#define KY_VERSION_NUMSTR KY_VERSION_MAJOR "." KY_VERSION_MINOR "." KY_VERSION_PATCH "-" KY_VERSION_STAGE
#define KY_VERSION_FULL   "🐖 kyanite " KY_VERSION_NUMSTR

/* output */
#define KY_OUT_NOERR    "🎉 no errors"
#define KY_OUT_ERR      "💀 uh oh!"
#define KY_OUT_ERRPARAM "👀 oops..."

/* types */
#define KY_TYPE_NIL      0
#define KY_TYPE_INT      1
#define KY_TYPE_FLOAT    2
#define KY_TYPE_BOOL     3
#define KY_TYPE_STRING   4  /* here onwards are object-able */
#define KY_TYPE_LIST     5
#define KY_TYPE_OBJECT   6
#define KY_TYPE_FUNCTION 7

/* types that can be checked - see kylex.c */
#define KY_FIRST_CHECKABLE_TYPE KY_TYPE_INT
#define KY_NUMCHECKABLE_TYPES 5

/* type-checking macros */
#define _is_nil(t) (t == KY_TYPE_NIL)
#define _is_int(t) (t == KY_TYPE_INT)
#define _is_float(t) (t == KY_TYPE_FLOAT)
#define _is_string(t) (t == KY_TYPE_STRING)
#define _is_bool(t) (t == KY_TYPE_BOOL)
#define _is_list(t) (t == KY_TYPE_LIST)
#define _is_object(t) (t == KY_TYPE_OBJECT)
#define _is_function(t) (t == KY_TYPE_FUNCTION)

/* types for single bytes (e.g storing value type) */
typedef unsigned char kbyte;

/* float type */
#define KY_FLOAT       double
#define KY_FLOAT_FMT   "%.17g"

#ifdef LLONG_MAX

/* define int type if we have long long available */
#define KY_INT         long long
#define KY_INT_FMT     "%d"
#define KY_INT_MAX     LLONG_MAX

#else
#error "Compiler does not support Kyanite's int representation (long long). See kyanite.h for details."
#endif

/* type definitions for int and float representations */
typedef KY_INT   kint;
typedef KY_FLOAT kfloat;

#define KTRUE  1
#define KFALSE 0
typedef unsigned char kbool;

/* types for memory used by kyanite */
typedef size_t kmem;      /* unsigned size */
typedef ptrdiff_t kmem_s; /* signed size */

/* cast macro */
#define cast(t, e) ((t)(e))

/* type for C functions that interact with kyanite */
typedef int (*kf) (ky_state_t *k);                     /* generic interaction */
typedef int (*kfc) (ky_state_t *k, ky_call_info_t *c); /* kyanite function with C backend */
typedef void * (*kf_alloc) (void *p, size_t n);        /* memory allocation function */

/* statuses */
#define KY_GOOD    0 /* all good :) */
#define KY_ERR_SYN 1 /* syntax error */
#define KY_ERR_RUN 2 /* runtime error */
#define KY_ERR_MEM 3 /* memory exception */
#define KY_ERR_OTH 4 /* other exception */

typedef unsigned char kstatus; /* type of status/error */

/* hash type */
typedef unsigned long khash;

/* types for parameters */
#define KY_MAX_PARAM 64
typedef unsigned char knparam;

/* avoid warnings */
#define KUNUSED(x) ((void)(x))

#endif