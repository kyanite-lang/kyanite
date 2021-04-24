#ifndef kyanite_h
#define kyanite_h

#include <limits.h>

/* version */
#define KY_VERSION_MAJOR "0"
#define KY_VERSION_MINOR "0"
#define KY_VERSION_PATCH "1"
#define KY_VERSION_STAGE "alpha"

#define KY_VERSION_NUM 1
#define KY_VERSION_FULL "🐖 kyanite " KY_VERSION_MAJOR "." KY_VERSION_MINOR "." KY_VERSION_PATCH "-" KY_VERSION_STAGE

/* output */
#define KY_OUT_NOERR    "🎉 no errors"
#define KY_OUT_ERR      "💀 uh oh!"
#define KY_OUT_ERRPARAM "👀 oops..."

/* types */
#define KY_TYPE_NIL      0
#define KY_TYPE_INT      1
#define KY_TYPE_FLOAT    2
#define KY_TYPE_STRING   3
#define KY_TYPE_BOOL     4
#define KY_TYPE_LIST     5
#define KY_TYPE_OBJECT   6
#define KY_TYPE_FUNCTION 7

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

#endif