#pragma once

#include <inttypes.h>
#include <stddef.h>
#include "pustr_macros.h"

#define NTYPES 0
#define TRACE_START 1

#ifdef NPUSTR
#define pusnprintf(...) 1
#define puprintf(...) 1
#else

typedef size_t (*typed_snprint)(char *, size_t, const void *);

typedef const struct{
    const void *val;
    typed_snprint typed_snprint;
}pu_arg;

#define pusnprintf(b, l, fmt, as...)            \
    _pusnprintf(b, l, fmt, pu_args_of(as))
size_t _pusnprintf(char *b, size_t max, const char *fmt, const pu_arg *args);

#define puprintf(fmt, as...)                    \
    _puprintf(fmt, pu_args_of(as))
int _puprintf(const char *fmt, const pu_arg *args);

#define pu_args_of(as...)                       \
    ((pu_arg []){ PUMAP(pu_arg_of, _, as) })

#define pu_arg_of(a, _,  __)                                \
    (pu_arg){                                               \
        .val = (const typeof(strip_bitfield(a))[1]) {a},    \
        .typed_snprint = (typed_snprint) pusnprint_of(a)    \
    }

#define pusnprint_of(a)                                     \
    _Generic(strip_bitfield(a),                             \
             PUMAP2(assoc_of_dflt, _, DEFAULT_TYPES),       \
             REPEAT_NOCOMMA(assoc_of_custom, _, NTYPES)     \
    default:                                                \
             pusnprint_dflt)

#define strip_bitfield(a) (0 ? (a) : (a))

#define assoc_of_dflt(t, _, __)                                         \
        t                 : &PU_CONCAT(pusnprint_, t),                     \
        volatile t        : &PU_CONCAT(pusnprint_, t),                     \
        const t           : &PU_CONCAT(pusnprint_, t),                     \
        volatile const t  : &PU_CONCAT(pusnprint_, t),                     \
        t*                : &PU_CONCAT(pusnprint_ptr_, t),                 \
        volatile t*       : &PU_CONCAT(pusnprint_ptr_, t),                 \
        const t*          : &PU_CONCAT(pusnprint_ptr_, t),                 \
        volatile const t* : &PU_CONCAT(pusnprint_ptr_, t)

#define assoc_of_custom(i, _) _assoc_of_custom(PU_CONCAT(putype_, i), i)
#define _assoc_of_custom(t, i)                                          \
        t                 : &PU_CONCAT(pusnprint_, i),                     \
        volatile t        : &PU_CONCAT(pusnprint_, i),                     \
        const t           : &PU_CONCAT(pusnprint_, i),                     \
        volatile const t  : &PU_CONCAT(pusnprint_, i),                     \
        t*                : &PU_CONCAT(pusnprint_ptr_, i),                 \
        volatile t*       : &PU_CONCAT(pusnprint_ptr_, i),                 \
        const t*          : &PU_CONCAT(pusnprint_ptr_, i),                 \
        volatile const t* : &PU_CONCAT(pusnprint_ptr_, i),

#define DEFAULT_TYPES                                       \
    _Bool, int8_t, int16_t, int32_t, int64_t,               \
    uint8_t, uint16_t, uint32_t, uint64_t, double, char

#define puprot(t)                                                       \
    size_t PU_CONCAT(pusnprint_, t)(char *b, size_t l, const t *a);     \
    size_t PU_CONCAT(pusnprint_ptr_, t)(char *b, size_t l, const t **a) \

puprot(_Bool);
puprot(int8_t);
puprot(int16_t);
puprot(int32_t);
puprot(int64_t);
puprot(uint8_t);
puprot(uint16_t);
puprot(uint32_t);
puprot(uint64_t);
puprot(double);
puprot(char);

size_t pusnprint_dflt(char *b, size_t l, const void **a);

#endif

