#ifndef kyobj_h
#define kyobj_h

#include "kyanite.h"

/* gc-able object (object, list, string) */
typedef struct {
    ky_object_t *next;
    kbyte        type;
} ky_object_t;

/* possible types of value */
typedef union {
    ky_object_t *kobject;
    kint         kint;
    kfloat       kfloat;
    kbool        kbool;
} ky_value_u;
/* value of object */
typedef struct {
    ky_value_u value;
    kbyte      type;
} ky_value_t;

/* macros for getting values from ky_value_t */
#define _kint(v) (v.value.kint)
#define _kfloat(v) (v.value.kfloat)
#define _kbool(v) (v.value.kbool)
#define _kobj(v) (v.value.kobject)

/* macros for determining whether a type has an object or real value */
#define _is_obj_value(t) (t == KY_TYPE_STRING || t == KY_TYPE_OBJECT || t == KY_TYPE_LIST)
#define _is_real_value(t) (!(_is_obj_value(t)))

#endif