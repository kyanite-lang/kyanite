#ifndef kyanite_h
#define kyanite_h

#include <limits.h>

#define KY_VERSION_MAJOR "0"
#define KY_VERSION_MINOR "0"
#define KY_VERSION_PATCH "1"
#define KY_VERSION_STAGE "alpha"

#define KY_VERSION_FULL "🐖 kyanite " KY_VERSION_MAJOR "." KY_VERSION_MINOR "." KY_VERSION_PATCH "-" KY_VERSION_STAGE

#define KY_OUT_NOERR    "🎉 no errors"
#define KY_OUT_ERR      "💀 uh oh!"
#define KY_OUT_ERRPARAM "👀 oops..."

#define KY_TYPE_NIL    0
#define KY_TYPE_INT    1
#define KY_TYPE_FLOAT  2
#define KY_TYPE_STRING 3
#define KY_TYPE_BOOL   4
#define KY_TYPE_LIST   5

#define KY_FLOAT       double
#define KY_FLOAT_FMT   "%.17g"

#ifdef LLONG_MAX

#define KY_INT         long long
#define KY_INT_FMT     "%d"
#define KY_INT_MAX     LLONG_MAX

#else
#error "Compiler does not support Kyanite's int representation (long long). See kyanite.h for details."
#endif

#endif