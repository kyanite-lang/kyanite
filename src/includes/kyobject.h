#ifndef kyobj_h
#define kyobj_h

#include "kyanite.h"

/* common GC-able object header to allow inter-conversion */
#define k_object_header struct ky_object_t *next; kbyte type;

/**
 * Represents a GC-able object
 * i.e string, list, object, etc.
 * These types are castable from the object via
 * the shared object header.
 **/
typedef struct ky_object_t {
    k_object_header
} ky_object_t;

/* special marker of string */
typedef unsigned short ksspec;

/**
 * Represents a string
 **/
typedef struct ky_string_t {
    k_object_header
    size_t              size;    /* size of string */
    khash               hash;    /* string hash */
    kbyte               special; /* special marker */
    kbyte               extra;   /* extra information relating to above marker */
    struct ky_string_t *chain;    /* next string in hash chain */
    char                cs[1];   /* start of char stream */
} ky_string_t;

/* useful string macros */
#define kstr(s) ((s)->cs)

/* string markers (first 8 bits) */
#define KSTR_STATIC   1
#define KSTR_KEYWORD  2
#define KSTR_TYPENAME 3

/* macro for checking if a string is GC-able */
#define _kstr_collectable(s) ((s)->special < KSTR_STATIC)

/* macro for checking if a string is a reserved keyword */
#define _kstr_keyword(s) ((s)->special == KSTR_KEYWORD)

/* macro for checking if a string is a reserved type name */
#define _kstr_typename(s) ((s)->special == KSTR_TYPENAME)

/**
 * Represents a language object (like a class),
 * referred to in code as a map to differentiate
 * from the base object (any GC-able data structure)
 **/
typedef struct ky_map_t {
    k_object_header
    /* TODO */
} ky_map_t;

/* forward definition */
typedef struct ky_value_t ky_value_t;

/**
 * Represents a list
 **/
typedef struct ky_list_t {
    k_object_header
    size_t      n_entries;
    ky_value_t *entries;
} ky_list_t;

/**
 * Union allowing conversion between GC-able objects
 * and their individual real type representations.
 **/
typedef union {
    ky_object_t o;
    ky_string_t str;
    ky_map_t    map; 
    /* ...and more (TODO) */
} ky_object_u;

/* macros for conversion of GC-able objects to sub-types */
#define kobj_u(o) cast(ky_object_u *, (o))
#define ko2str(o) &((kobj_u(o))->str)

/* macros for determining size of object to allocate */
#define ksize_str(l) (offsetof(ky_string_t, cs) + ((l) + 1 * sizeof(char)))

/* possible types of value */
typedef union {
    ky_object_t *kobject;
    kint         kint;
    kfloat       kfloat;
    kbool        kbool;
} ky_value_u;

/* value of object */
typedef struct ky_value_t {
    ky_value_u value;
    kbyte      type;
} ky_value_t;

/* macros for getting values from ky_value_t */
#define _kint(v) ((v).value.kint)
#define _kfloat(v) ((v).value.kfloat)
#define _kbool(v) ((v).value.kbool)
#define _kobj(v) ((v).value.kobject)

/* macros for determining whether a type has an object or real value */
#define _is_obj_value(t) (t == KY_TYPE_STRING || t == KY_TYPE_OBJECT || t == KY_TYPE_LIST)
#define _is_real_value(t) (!(_is_obj_value(t)))

ky_object_t* ky_object_new(ky_state_t *k, kbyte type, size_t size);

void ky_object_free(ky_state_t *k, ky_object_t *o);

#endif