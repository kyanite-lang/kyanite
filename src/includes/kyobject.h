#ifndef kyobj_h
#define kyobj_h

#include "kyanite.h"

/**
 * common header for GC-able objects
 * next = next object in o_all list
 * type = object type
 * gcm  = status of GC (black/grey/white)
 **/
#define k_object_header struct ky_object_t *next; \
                        kbyte type; \
                        kbyte gcm;

#define KY_GCM_BLACK  0 /* do not collect */
#define KY_GCM_GREY   1 /* do not collect; scan for references */
#define KY_GCM_WHITE  2 /* collectable */

/**
 * Represents a GC-able object
 * i.e string, list, object, etc.
 * These types are castable from the object via
 * the shared object header.
 **/
typedef struct ky_object_t {
    k_object_header
} ky_object_t;

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
#define _is_obj_value(t) (t == KY_TYPE_STRING || t == KY_TYPE_OBJECT || t == KY_TYPE_LIST || t == KY_TYPE_FUNCTION)
#define _is_real_value(t) (!(_is_obj_value(t)))

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
#define KSTR_EXEMPT   2 /* generically exempt from GC */
#define KSTR_KEYWORD  3 /* language keyword */
#define KSTR_TYPENAME 4 /* language typename */

/* macro for checking if a string is GC-able */
#define _kstr_collectable(s) ((s)->special < KSTR_EXEMPT)

/* macro for checking if a string is a reserved keyword */
#define _kstr_keyword(s) ((s)->special == KSTR_KEYWORD)

/* macro for checking if a string is a reserved type name */
#define _kstr_typename(s) ((s)->special == KSTR_TYPENAME)

/**
 * Represents a node on a tree of qualified identifiers
 **/
typedef struct ky_ident_node_t {
    struct ky_ident_node_t *next;   /* next in this subtree */
    struct ky_ident_node_t *child;  /* first child of this node */
    struct ky_ident_node_t *parent; /* parent of this node */
    ky_string_t            *ident;  /* string for this bit of ident */
} ky_ident_node_t;

/* definitions for kinds of identifiers */
#define KY_IDENT_NAKED     0
#define KY_IDENT_QUALIFIED 1

/**
 * Represents an identifier as usable in the language
 **/
typedef struct ky_ident_t {
    kbyte idt; /* KY_IDENT_QUALIFIED or KY_IDENT_NAKED */
    union {
        ky_ident_node_t *idn;
        ky_string_t     *str;
    } u;
} ky_ident_t;

/**
 * Represents a language object (like a class),
 * referred to in code as a map to differentiate
 * from the base object (any GC-able data structure)
 **/
typedef struct ky_map_proto_t {
    k_object_header
    kmem          n_attrib; /* number of attributes (variables) */
    ky_string_t **attrib;   /* attribute identifiers (names) */
} ky_map_proto_t;

/**
 * Represents an instance of a map
 **/
typedef struct ky_map_t {
    k_object_header
    ky_map_proto_t *proto;  /* the map proto this is instance of */
    ky_value_t     *values; /* values for each attribute */
} ky_map_t;

/**
 * Represents a list
 **/
typedef struct ky_list_t {
    k_object_header
    size_t      n_entries;
    ky_value_t *entries;
} ky_list_t;

/**
 * Represents an instruction that can be followed
 * by the interpreter
 **/
typedef struct ky_instruction_t {
    kbyte         op;
    unsigned long info;
    int           linenum;
} ky_instruction_t;

/* function definitions */
#define KY_FUNC_KY     0 /* native kyanite function with set parameters */
#define KY_FUNC_KY_VAR 1 /* native kyanite function with variable parameters */
#define KY_FUNC_C      2 /* C function with set parameters */
#define KY_FUNC_C_VAR  3 /* C function with variable parameters */

/**
 * Represents a function in the language (or the root program)
 * in case of variable params, n_param is minimum
 **/
typedef struct ky_method_t {
    k_object_header
    kbyte             func_type;  /* function type (see above definitions) */
    unsigned short    n_params;   /* number of parameters used by function */
    int               line;       /* line the function is defined on */
    union {
        kfc c;                         /* C function to pass data to */
        struct {
            kmem              n_code;  /* number of instructions in the function */
            ky_instruction_t *code;    /* array of instructions */
        } ky;
    } u;
} ky_method_t;

/**
 * Union allowing conversion between GC-able objects
 * and their individual real type representations.
 **/
typedef union {
    ky_object_t o;
    ky_string_t str;
    ky_method_t fnc;
    ky_map_t    map; 
    /* ...and more (TODO) */
} ky_object_u;

/* macros for conversion of GC-able objects to sub-types */
#define kobj_u(o) cast(ky_object_u *, (o))
#define ko2str(o) (&((kobj_u(o))->str))
#define ko2fnc(o) (&((kobj_u(o))->fnc))
#define ko2map(o) (&((kobj_u(o))->map))
#define kany2o(a) (&((kobj_u(a))->o))

/* macros for determining size of object to allocate */
#define ksize_str(l) (offsetof(ky_string_t, cs) + ((l) + 1 * sizeof(char)))

/* macros for inline creating values */
#define _kv_int_new(i) ((ky_value_t){ .value.kint = ((kint)i), .type = KY_TYPE_INT })
#define _kv_float_new(f) ((ky_value_t){ .value.kfloat = ((kfloat)f), .type = KY_TYPE_FLOAT })
#define _kv_bool_new(b) ((ky_value_t){ .value.kbool = ((kbool)((b > 0) ? KTRUE : KFALSE)), .type = KY_TYPE_BOOL })
#define _kv_str_new(s) ((ky_value_t){ .value.kobject = kany2o(s), .type = KY_TYPE_STRING })

ky_object_t* ky_object_new(ky_state_t *k, kbyte type, size_t size);
void ky_object_free(ky_state_t *k, ky_object_t *o);

ky_ident_node_t* ky_ident_new(ky_state_t* k, ky_string_t* s);
ky_ident_node_t* ky_ident_find_or_create(ky_state_t* k, ky_ident_node_t** previous, ky_string_t* s);
ky_ident_node_t* ky_ident_find(ky_state_t* k, ky_ident_node_t* first, ky_string_t* s);
void ky_ident_insert(ky_state_t* k, ky_ident_node_t** previous, ky_ident_node_t* node);
void ky_ident_free(ky_state_t* k, ky_ident_node_t* node);

ky_map_proto_t* ky_map_proto_new(ky_state_t* k, unsigned short n_param);

#endif