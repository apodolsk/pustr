#pragma once
#include <dial_macros.h>

#define CASSERT(e) _Static_assert(e, #e)
/* Safe in expressions, wherein _Static_assert is incorrect because it's a declaration. */
#define CASSERT_EXP(e) ((void)sizeof(char[1 - 2*!(e)]))

#define ARR_LEN(a) (sizeof(a)/sizeof(*(a)))
#define ARR_LEN_2D(a) (sizeof(a)/sizeof(**(a)))

/** 
 * If p, &ret->field == p. Iff !p, ret == NULL.
 */
#define cof container_of
#define container_of(p, container_t, field)                             \
    ((container_t *)                                                    \
     subtract_if_not_null((uptr) (p), offsetof(container_t, field)))

/* Used to do a NULL check without expanding p twice. */
static inline void *subtract_if_not_null(uptr p, cnt s){
    return (void *) (p ? (p - s) : p);
}

#define cof_aligned_pow2(p, container_t)                           \
    ((container_t *) align_down_pow2(p, sizeof(container_t)))

/* Record update. For each arg of form '.f e' in changes, ret.f == e. If
   field f' doesn't occur in args, ret.f' == orig.f'. */
#define RUP_PFX(fld,_, __) __rup_copy fld
#define rup(orig, changes...)({                 \
            typeof(orig) __rup_copy = orig;     \
            MAP(RUP_PFX,, changes);             \
            __rup_copy;                         \
        })                                      \


#define tryp(p, catch...)({                     \
            typeof(p) try_p = p;                \
            if(!try_p){                         \
                catch;                          \
            }                                   \
            try_p;                              \
        })                                      \


/* Clang has a buggy statement-expression implementation. */
/* #define PUN(t, s) ({                                                \ */
/*         CASSERT(sizeof(s) == sizeof(t));                            \ */
/*         ((union {__typeof__(s) str; t i;}) (s)).i;                  \ */
/*         })                                                       */

/* #define PUN(t, s)                                               \ */
/*     (assert(sizeof(s) == sizeof(t)),                            \ */
/*      ((union {__typeof__(s) str; t i;}) (s)).i) */

#define PUN(t, s)                                               \
    (((union {__typeof__(s) str; t i;}) (s)).i)

/* Execute (first, as) with comma operator sequencing, except evaluate to
   first rather than the last argument. */
#define seq_first(first, as...) ({              \
            typeof(first) __first = first;      \
            0, ##as;                            \
            __first;                            \
        })


/* #define PUN(t, s) (*(t*)(typeof(s)[]){s}) */

#define eq(a, b) ({ typeof(b) __eqa = a; (PUN(uptr, __eqa) == PUN(uptr, b)); })

#define swap(ap, bp)({                             \
            typeof(*(ap)) __swap_tmp = *(ap);      \
            *(ap) = *(bp);                         \
            *(bp) = __swap_tmp;                    \
    })                                             \


/* #define eq2(a, b) ({ typeof(b) __eq2a = a; (PUN(dptr, __eq2a) == PUN(dptr, b)); }) */
#define eq2(a, b) _eq2(PUN(dptr, a), PUN(dptr, b))
static inline bool _eq2(dptr a, dptr b){
    return a == b;
}

#define umin(a, b) _umin((uptr) (a), (uptr) (b))
static inline uptr _umin(uptr a, uptr b){
    return a < b ? a : b;
}

#define umax(a, b) _umax((uptr) (a), (uptr) (b))
static inline uptr _umax(uptr a, uptr b){
    return a < b ? b : a;
}

static int must(int i){
    assert(i);
    return i;
}

static err muste(err e){
    assert(!e);
    return e;
}

static void *mustp(void *p){
    assert(p);
    return p;
}

#define in_struct(p, s)                                         \
    ((uptr) (p) >= (uptr) (s) && (uptr) (p) < (uptr)((s) + 1))  \

#define in_array(p, a)                                                 \
    ((uptr) (p) >= (uptr) (s) && (uptr) (p) < (uptr)(s) + sizeof(s))   \

#define is_pow2(n) ((n) && !((n) & ((n) - 1)))

/* static inline bool is_pow2(uint n){ */
/*     return n && !(n & (n - 1)); */
/* } */

static inline uint div_pow2(uint n, uint by){
    if(!n)
        return 0;
    assert(is_pow2(by));
    return n >> __builtin_ctz(by);
}

static inline uint div_rup_pow2(uint n, uint by){
    if(!n)
        return 0;
    assert(is_pow2(by));
    uint q = n >> __builtin_ctz(by);
    bool r_notzero = !!(n & (by - 1));
    return q + r_notzero;
}

#define div_rup(a, by)                          \
    ((a) + ((by) - 1) / (by))                   

#define aligned(addr, size)                     \
    (((uptr)(addr) % (size)) == 0)

#define align_down(addr, size)                              \
    ((typeof(addr)) ualign_down((uptr) (addr), (size)))
static inline uptr ualign_down(uptr addr, size size){
    return addr - addr % size;
}

#define align_up(addr, size)                    \
    ((typeof(addr)) ualign_up((uptr) (addr), (size)))
static inline uptr ualign_up(uptr addr, size size){
    return ualign_down(addr + size - 1, size);
}

#define align_down_pow2(n, size)                  \
    ((typeof (n)) (assert(is_pow2(size)),         \
                   (uptr) (n) & ~((size) - 1)))

#define align_up_pow2(n, size)                                          \
    (((typeof (n)) (assert(is_pow2(size)),                              \
                    align_down_pow2((uptr) (n) + (size) - 1, size))))

#define const_align_down_pow2(n, size)            \
    ((typeof (n)) (CASSERT_EXP(is_pow2(size)),    \
                   (uptr) (n) & ~((size) - 1)))

#define const_align_up_pow2(n, size)                                    \
    ((typeof (n)) (CASSERT_EXP(is_pow2(size)),                          \
                   const_align_down_pow2((uptr) (n) + (size) - 1, size)))

#define mod_pow2(n, mod)                        \
    ((uptr) (n) & ((mod) - 1))

#define aligned_pow2(n, size)                   \
    (!mod_pow2(n, size))

#define limit(n, ceil){                         \
        typeof(n) __limit_n = n;                \
        typeof(n) __limit_ceil = ceil;          \
        if(__ceil_n >= __limit_ceil)            \
            __limit_ceil - 1;                   \
        else                                    \
            __ceil_n;                           \
    }                                           \

/* TODO: apparently not working */
/* #define align_next_pow2(n)                                              \ */
/*     ({                                                                  \ */
/*         CASSERT((sizeof(void *)/sizeof((n, 0))) * sizeof((n, 0))        \ */
/*               == sizeof(void *));                                       \ */
/*         1 << ((WORDBITS * sizeof((n, 0)) / sizeof(void *))              \ */
/*               - __builtin_clz((n, 0) - 1));                             \ */
/*     })                                                                  \ */

static inline void no_op(){}
static inline err zero(){return 0;}

static inline bool ptr_overflow(const void *b, cnt off){
    return 0 - (uptr) b < off;
}

char* stpcpy(char *dest, const char *src);
