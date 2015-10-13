#pragma once

#include <dial_macros.h>

#include <inttypes.h>
#include <stdarg.h>

#define NTYPES 0

#ifdef NPUSTR
#define pudef_dflt(...)
#define pusnprintf(...) 1
#define puprintf(...) 1
#define putrace(_, f, as...) f(as)
#else

typedef size_t (*typed_snprint)(char *, size_t, volatile const void *);

typedef struct{
    volatile const void *val;
    typed_snprint typed_snprint;
}pu_arg;

size_t puvsnprintf(char *b, size_t max, const char *fmt, va_list args);

#define puprintf(fmt, as...)                                            \
    _puprintf(fmt COMMAPFX_IF_NZ(PUMAP(pu_arg_of, _, as)))
size_t _puprintf(const char *fmt, ...);

#define pusnprintf(b, l, fmt, as...)                \
    _pusnprintf(b, l, fmt, PUMAP(pu_arg_of, _, as))
size_t _pusnprintf(char *b, size_t max, const char *fmt, ...);

#define pu_arg_of(a, _,  __)                                             \
    &(pu_arg){(typeof(decay(a))[1]){a}, (typed_snprint) pusnprint_of(a)}

#define pusnprint_of(a)                                     \
    _Generic(decay(a),                                      \
             PUMAP2(pusnprint_of_dflt, _, DEFAULT_TYPES),   \
    default:                                                \
             REPEAT_NOCOMMA(LCHOOSE, decay(a), NTYPES)      \
             pusnprint_dflt                                 \
             REPEAT_NOCOMMA(RCHOOSE, decay(a), NTYPES))     \
    
#define pusnprint_of_dflt(t, _, __)                                     \
    t  : &CONCAT(pusnprint_, t),                                        \
        volatile t        : &CONCAT(pusnprint_, t),                     \
        const t           : &CONCAT(pusnprint_, t),                     \
        volatile const t  : &CONCAT(pusnprint_, t),                     \
        t*                : &CONCAT(pusnprint_ptr_, t),                 \
        volatile t*       : &CONCAT(pusnprint_ptr_, t),                 \
        const t*          : &CONCAT(pusnprint_ptr_, t),                 \
        volatile const t* : &CONCAT(pusnprint_ptr_, t)

/* TODO: I have to do this because gcc 4.9 has a bug where _Generic(b,
   typeof(b): ...) fails to match if b has a volatile type. */
#define LCHOOSE(i, a)                                                   \
           choose(compat(CONCAT(putype_, i),                            \
                         typeof(a)), &CONCAT(pusnprint_, i) ,           \
           choose(compat(volatile CONCAT(putype_, i),                   \
                         typeof(a)), &CONCAT(pusnprint_, i) ,           \
           choose(compat(CONCAT(putype_, i) *,                          \
                         typeof(a)), &CONCAT(pusnprint_ptr_, i) ,       \
           choose(compat(volatile CONCAT(putype_, i) *,                 \
                         typeof(a)), &CONCAT(pusnprint_ptr_, i) ,       \
                                                                        \

#define RCHOOSE(i, _) ))))

/* TODO: This is what it should be, were it not for that bug. */
/* #define pusnprint_of_scoped(i, _)                                   \ */
/*     typeof(decay((CONCAT(putype_, i)){})): &CONCAT(pusnprint_, i)                   */


/* gcc and clang don't allow 'void' to occur in the list of _Generic
   types, but do allow it in __builtin_types_compatible_p. I use this to
   allow putrace to report and propogate the value of fun(as) iff
   typeof(fun(as)) != void. */
#define choose __builtin_choose_expr
#define compat __builtin_types_compatible_p
#define is_void(e) compat(void, typeof(e))

/* To avoid double-eval of function args. */
#define pu_strfmt(_, __, ___) %

#define putrace(print, fun, as...)                                      \
    ({                                                                  \
        PUMAP_NOCOMMA(estore, __pu, as);                                \
        print("-- Begin %(" STRLIT(PUMAP(pu_strfmt, _, as))             \
              ") in %:%", #fun                                          \
              COMMAPFX_IF_NZ(PUMAP3(eref, __pu, as)),                   \
              __func__, __LINE__);                                      \
        typeof(choose(is_void(fun(as)),                                 \
                      1,                                                \
                      fun(as)))                                         \
            __pu_ret;                                                   \
        __pu_ret =                                                      \
        choose(is_void(fun(as)),                                        \
               (fun(PUMAP(eref, __pu, as)), 0),                            \
               fun(PUMAP(eref, __pu, as)));                             \
        choose(is_void(fun(as)),                                        \
               ({                                                       \
                   print("-- End %() in %:%:%", #fun,                   \
                         __FILE__, __func__, __LINE__);                 \
               }),                                                      \
               ({                                                       \
                   print("-- End % = %("STRLIT(PUMAP(pu_strfmt, _, as)) \
                         ") in %:%:%", __pu_ret, #fun                   \
                         COMMAPFX_IF_NZ(PUMAP3(eref, __pu, as)),        \
                         __FILE__, __func__, __LINE__);                 \
                   __pu_ret;                                            \
               }));                                                     \
    })

#define DEFAULT_TYPES                                       \
    bool, int8_t, int16_t, int32_t, int64_t,                \
    uint8_t, uint16_t, uint32_t, uint64_t, double, char

#define puprot(t)                                                       \
    size_t CONCAT(pusnprint_, t)(char *b, size_t l, volatile const t *a); \
    size_t CONCAT(pusnprint_ptr_, t)(char *b, size_t l, volatile const t **a)

puprot(bool);
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

size_t pusnprint_dflt(char *b, size_t l, volatile const void **a);

#define PUMAP(FUNC, global, ...) CONCAT(PUMAP_ , NUM_ARGS(__VA_ARGS__)) \
    (FUNC, global, __VA_ARGS__)

#define PUMAP_30(f, g, arg, ...) f(arg, g, 29), PUMAP_29(f, g, __VA_ARGS__)
#define PUMAP_29(f, g, arg, ...) f(arg, g, 28), PUMAP_28(f, g, __VA_ARGS__)
#define PUMAP_28(f, g, arg, ...) f(arg, g, 27), PUMAP_27(f, g, __VA_ARGS__)
#define PUMAP_27(f, g, arg, ...) f(arg, g, 26), PUMAP_26(f, g, __VA_ARGS__)
#define PUMAP_26(f, g, arg, ...) f(arg, g, 25), PUMAP_25(f, g, __VA_ARGS__)
#define PUMAP_25(f, g, arg, ...) f(arg, g, 24), PUMAP_24(f, g, __VA_ARGS__)
#define PUMAP_24(f, g, arg, ...) f(arg, g, 23), PUMAP_23(f, g, __VA_ARGS__)
#define PUMAP_23(f, g, arg, ...) f(arg, g, 22), PUMAP_22(f, g, __VA_ARGS__)
#define PUMAP_22(f, g, arg, ...) f(arg, g, 21), PUMAP_21(f, g, __VA_ARGS__)
#define PUMAP_21(f, g, arg, ...) f(arg, g, 20), PUMAP_20(f, g, __VA_ARGS__)
#define PUMAP_20(f, g, arg, ...) f(arg, g, 19), PUMAP_19(f, g, __VA_ARGS__)
#define PUMAP_19(f, g, arg, ...) f(arg, g, 18), PUMAP_18(f, g, __VA_ARGS__)
#define PUMAP_18(f, g, arg, ...) f(arg, g, 17), PUMAP_17(f, g, __VA_ARGS__)
#define PUMAP_17(f, g, arg, ...) f(arg, g, 16), PUMAP_16(f, g, __VA_ARGS__)
#define PUMAP_16(f, g, arg, ...) f(arg, g, 15), PUMAP_15(f, g, __VA_ARGS__)
#define PUMAP_15(f, g, arg, ...) f(arg, g, 14), PUMAP_14(f, g, __VA_ARGS__)
#define PUMAP_14(f, g, arg, ...) f(arg, g, 13), PUMAP_13(f, g, __VA_ARGS__)
#define PUMAP_13(f, g, arg, ...) f(arg, g, 12), PUMAP_12(f, g, __VA_ARGS__)
#define PUMAP_12(f, g, arg, ...) f(arg, g, 11), PUMAP_11(f, g, __VA_ARGS__)
#define PUMAP_11(f, g, arg, ...) f(arg, g, 10), PUMAP_10(f, g, __VA_ARGS__)
#define PUMAP_10(f, g, arg, ...) f(arg, g, 9), PUMAP_9(f, g, __VA_ARGS__)
#define PUMAP_9(f, g, arg, ...) f(arg, g, 8), PUMAP_8(f, g, __VA_ARGS__)
#define PUMAP_8(f, g, arg, ...) f(arg, g, 7), PUMAP_7(f, g, __VA_ARGS__)
#define PUMAP_7(f, g, arg, ...) f(arg, g, 6), PUMAP_6(f, g, __VA_ARGS__)
#define PUMAP_6(f, g, arg, ...) f(arg, g, 5), PUMAP_5(f, g, __VA_ARGS__)
#define PUMAP_5(f, g, arg, ...) f(arg, g, 4), PUMAP_4(f, g, __VA_ARGS__)
#define PUMAP_4(f, g, arg, ...) f(arg, g, 3), PUMAP_3(f, g, __VA_ARGS__)
#define PUMAP_3(f, g, arg, ...) f(arg, g, 2), PUMAP_2(f, g, __VA_ARGS__)
#define PUMAP_2(f, g, arg, ...) f(arg, g, 1), PUMAP_1(f, g, __VA_ARGS__)
#define PUMAP_1(f, g, arg) f(arg, g, 0)
#define PUMAP_0(f, g, arg)

#define PUMAP2(FUNC, global, ...) CONCAT(PUMAP2_ , NUM_ARGS(__VA_ARGS__)) \
    (FUNC, global, __VA_ARGS__)

#define PUMAP2_30(f, g, arg, ...) f(arg, g, 29), PUMAP2_29(f, g, __VA_ARGS__)
#define PUMAP2_29(f, g, arg, ...) f(arg, g, 28), PUMAP2_28(f, g, __VA_ARGS__)
#define PUMAP2_28(f, g, arg, ...) f(arg, g, 27), PUMAP2_27(f, g, __VA_ARGS__)
#define PUMAP2_27(f, g, arg, ...) f(arg, g, 26), PUMAP2_26(f, g, __VA_ARGS__)
#define PUMAP2_26(f, g, arg, ...) f(arg, g, 25), PUMAP2_25(f, g, __VA_ARGS__)
#define PUMAP2_25(f, g, arg, ...) f(arg, g, 24), PUMAP2_24(f, g, __VA_ARGS__)
#define PUMAP2_24(f, g, arg, ...) f(arg, g, 23), PUMAP2_23(f, g, __VA_ARGS__)
#define PUMAP2_23(f, g, arg, ...) f(arg, g, 22), PUMAP2_22(f, g, __VA_ARGS__)
#define PUMAP2_22(f, g, arg, ...) f(arg, g, 21), PUMAP2_21(f, g, __VA_ARGS__)
#define PUMAP2_21(f, g, arg, ...) f(arg, g, 20), PUMAP2_20(f, g, __VA_ARGS__)
#define PUMAP2_20(f, g, arg, ...) f(arg, g, 19), PUMAP2_19(f, g, __VA_ARGS__)
#define PUMAP2_19(f, g, arg, ...) f(arg, g, 18), PUMAP2_18(f, g, __VA_ARGS__)
#define PUMAP2_18(f, g, arg, ...) f(arg, g, 17), PUMAP2_17(f, g, __VA_ARGS__)
#define PUMAP2_17(f, g, arg, ...) f(arg, g, 16), PUMAP2_16(f, g, __VA_ARGS__)
#define PUMAP2_16(f, g, arg, ...) f(arg, g, 15), PUMAP2_15(f, g, __VA_ARGS__)
#define PUMAP2_15(f, g, arg, ...) f(arg, g, 14), PUMAP2_14(f, g, __VA_ARGS__)
#define PUMAP2_14(f, g, arg, ...) f(arg, g, 13), PUMAP2_13(f, g, __VA_ARGS__)
#define PUMAP2_13(f, g, arg, ...) f(arg, g, 12), PUMAP2_12(f, g, __VA_ARGS__)
#define PUMAP2_12(f, g, arg, ...) f(arg, g, 11), PUMAP2_11(f, g, __VA_ARGS__)
#define PUMAP2_11(f, g, arg, ...) f(arg, g, 10), PUMAP2_10(f, g, __VA_ARGS__)
#define PUMAP2_10(f, g, arg, ...) f(arg, g, 9), PUMAP2_9(f, g, __VA_ARGS__)
#define PUMAP2_9(f, g, arg, ...) f(arg, g, 8), PUMAP2_8(f, g, __VA_ARGS__)
#define PUMAP2_8(f, g, arg, ...) f(arg, g, 7), PUMAP2_7(f, g, __VA_ARGS__)
#define PUMAP2_7(f, g, arg, ...) f(arg, g, 6), PUMAP2_6(f, g, __VA_ARGS__)
#define PUMAP2_6(f, g, arg, ...) f(arg, g, 5), PUMAP2_5(f, g, __VA_ARGS__)
#define PUMAP2_5(f, g, arg, ...) f(arg, g, 4), PUMAP2_4(f, g, __VA_ARGS__)
#define PUMAP2_4(f, g, arg, ...) f(arg, g, 3), PUMAP2_3(f, g, __VA_ARGS__)
#define PUMAP2_3(f, g, arg, ...) f(arg, g, 2), PUMAP2_2(f, g, __VA_ARGS__)
#define PUMAP2_2(f, g, arg, ...) f(arg, g, 1), PUMAP2_1(f, g, __VA_ARGS__)
#define PUMAP2_1(f, g, arg) f(arg, g, 0)
#define PUMAP2_0(f, g, arg)

#define PUMAP3(FUNC, global, ...) CONCAT(PUMAP3_ , NUM_ARGS(__VA_ARGS__)) \
    (FUNC, global, __VA_ARGS__)

#define PUMAP3_30(f, g, arg, ...) f(arg, g, 29), PUMAP3_29(f, g, __VA_ARGS__)
#define PUMAP3_29(f, g, arg, ...) f(arg, g, 28), PUMAP3_28(f, g, __VA_ARGS__)
#define PUMAP3_28(f, g, arg, ...) f(arg, g, 27), PUMAP3_27(f, g, __VA_ARGS__)
#define PUMAP3_27(f, g, arg, ...) f(arg, g, 26), PUMAP3_26(f, g, __VA_ARGS__)
#define PUMAP3_26(f, g, arg, ...) f(arg, g, 25), PUMAP3_25(f, g, __VA_ARGS__)
#define PUMAP3_25(f, g, arg, ...) f(arg, g, 24), PUMAP3_24(f, g, __VA_ARGS__)
#define PUMAP3_24(f, g, arg, ...) f(arg, g, 23), PUMAP3_23(f, g, __VA_ARGS__)
#define PUMAP3_23(f, g, arg, ...) f(arg, g, 22), PUMAP3_22(f, g, __VA_ARGS__)
#define PUMAP3_22(f, g, arg, ...) f(arg, g, 21), PUMAP3_21(f, g, __VA_ARGS__)
#define PUMAP3_21(f, g, arg, ...) f(arg, g, 20), PUMAP3_20(f, g, __VA_ARGS__)
#define PUMAP3_20(f, g, arg, ...) f(arg, g, 19), PUMAP3_19(f, g, __VA_ARGS__)
#define PUMAP3_19(f, g, arg, ...) f(arg, g, 18), PUMAP3_18(f, g, __VA_ARGS__)
#define PUMAP3_18(f, g, arg, ...) f(arg, g, 17), PUMAP3_17(f, g, __VA_ARGS__)
#define PUMAP3_17(f, g, arg, ...) f(arg, g, 16), PUMAP3_16(f, g, __VA_ARGS__)
#define PUMAP3_16(f, g, arg, ...) f(arg, g, 15), PUMAP3_15(f, g, __VA_ARGS__)
#define PUMAP3_15(f, g, arg, ...) f(arg, g, 14), PUMAP3_14(f, g, __VA_ARGS__)
#define PUMAP3_14(f, g, arg, ...) f(arg, g, 13), PUMAP3_13(f, g, __VA_ARGS__)
#define PUMAP3_13(f, g, arg, ...) f(arg, g, 12), PUMAP3_12(f, g, __VA_ARGS__)
#define PUMAP3_12(f, g, arg, ...) f(arg, g, 11), PUMAP3_11(f, g, __VA_ARGS__)
#define PUMAP3_11(f, g, arg, ...) f(arg, g, 10), PUMAP3_10(f, g, __VA_ARGS__)
#define PUMAP3_10(f, g, arg, ...) f(arg, g, 9), PUMAP3_9(f, g, __VA_ARGS__)
#define PUMAP3_9(f, g, arg, ...) f(arg, g, 8), PUMAP3_8(f, g, __VA_ARGS__)
#define PUMAP3_8(f, g, arg, ...) f(arg, g, 7), PUMAP3_7(f, g, __VA_ARGS__)
#define PUMAP3_7(f, g, arg, ...) f(arg, g, 6), PUMAP3_6(f, g, __VA_ARGS__)
#define PUMAP3_6(f, g, arg, ...) f(arg, g, 5), PUMAP3_5(f, g, __VA_ARGS__)
#define PUMAP3_5(f, g, arg, ...) f(arg, g, 4), PUMAP3_4(f, g, __VA_ARGS__)
#define PUMAP3_4(f, g, arg, ...) f(arg, g, 3), PUMAP3_3(f, g, __VA_ARGS__)
#define PUMAP3_3(f, g, arg, ...) f(arg, g, 2), PUMAP3_2(f, g, __VA_ARGS__)
#define PUMAP3_2(f, g, arg, ...) f(arg, g, 1), PUMAP3_1(f, g, __VA_ARGS__)
#define PUMAP3_1(f, g, arg) f(arg, g, 0)
#define PUMAP3_0(f, g, arg)

#define PUMAP_NOCOMMA(FUNC, global, ...)          \
    CONCAT(PUMAPNC_ , NUM_ARGS(__VA_ARGS__))(FUNC, global, __VA_ARGS__)

#define PUMAPNC_30(f, g, arg, ...) f(arg, g, 29) PUMAPNC_29(f, g, __VA_ARGS__)
#define PUMAPNC_29(f, g, arg, ...) f(arg, g, 28) PUMAPNC_28(f, g, __VA_ARGS__)
#define PUMAPNC_28(f, g, arg, ...) f(arg, g, 27) PUMAPNC_27(f, g, __VA_ARGS__)
#define PUMAPNC_27(f, g, arg, ...) f(arg, g, 26) PUMAPNC_26(f, g, __VA_ARGS__)
#define PUMAPNC_26(f, g, arg, ...) f(arg, g, 25) PUMAPNC_25(f, g, __VA_ARGS__)
#define PUMAPNC_25(f, g, arg, ...) f(arg, g, 24) PUMAPNC_24(f, g, __VA_ARGS__)
#define PUMAPNC_24(f, g, arg, ...) f(arg, g, 23) PUMAPNC_23(f, g, __VA_ARGS__)
#define PUMAPNC_23(f, g, arg, ...) f(arg, g, 22) PUMAPNC_22(f, g, __VA_ARGS__)
#define PUMAPNC_22(f, g, arg, ...) f(arg, g, 21) PUMAPNC_21(f, g, __VA_ARGS__)
#define PUMAPNC_21(f, g, arg, ...) f(arg, g, 20) PUMAPNC_20(f, g, __VA_ARGS__)
#define PUMAPNC_20(f, g, arg, ...) f(arg, g, 19) PUMAPNC_19(f, g, __VA_ARGS__)
#define PUMAPNC_19(f, g, arg, ...) f(arg, g, 18) PUMAPNC_18(f, g, __VA_ARGS__)
#define PUMAPNC_18(f, g, arg, ...) f(arg, g, 17) PUMAPNC_17(f, g, __VA_ARGS__)
#define PUMAPNC_17(f, g, arg, ...) f(arg, g, 16) PUMAPNC_16(f, g, __VA_ARGS__)
#define PUMAPNC_16(f, g, arg, ...) f(arg, g, 15) PUMAPNC_15(f, g, __VA_ARGS__)
#define PUMAPNC_15(f, g, arg, ...) f(arg, g, 14) PUMAPNC_14(f, g, __VA_ARGS__)
#define PUMAPNC_14(f, g, arg, ...) f(arg, g, 13) PUMAPNC_13(f, g, __VA_ARGS__)
#define PUMAPNC_13(f, g, arg, ...) f(arg, g, 12) PUMAPNC_12(f, g, __VA_ARGS__)
#define PUMAPNC_12(f, g, arg, ...) f(arg, g, 11) PUMAPNC_11(f, g, __VA_ARGS__)
#define PUMAPNC_11(f, g, arg, ...) f(arg, g, 10) PUMAPNC_10(f, g, __VA_ARGS__)
#define PUMAPNC_10(f, g, arg, ...) f(arg, g, 9) PUMAPNC_9(f, g, __VA_ARGS__)
#define PUMAPNC_9(f, g, arg, ...) f(arg, g, 8) PUMAPNC_8(f, g, __VA_ARGS__)
#define PUMAPNC_8(f, g, arg, ...) f(arg, g, 7) PUMAPNC_7(f, g, __VA_ARGS__)
#define PUMAPNC_7(f, g, arg, ...) f(arg, g, 6) PUMAPNC_6(f, g, __VA_ARGS__)
#define PUMAPNC_6(f, g, arg, ...) f(arg, g, 5) PUMAPNC_5(f, g, __VA_ARGS__)
#define PUMAPNC_5(f, g, arg, ...) f(arg, g, 4) PUMAPNC_4(f, g, __VA_ARGS__)
#define PUMAPNC_4(f, g, arg, ...) f(arg, g, 3) PUMAPNC_3(f, g, __VA_ARGS__)
#define PUMAPNC_3(f, g, arg, ...) f(arg, g, 2) PUMAPNC_2(f, g, __VA_ARGS__)
#define PUMAPNC_2(f, g, arg, ...) f(arg, g, 1) PUMAPNC_1(f, g, __VA_ARGS__)
#define PUMAPNC_1(f, g, arg) f(arg, g, 0)
#define PUMAPNC_0(f, g, arg)

#define REPEAT(f, g, lim) CONCAT(R_ , lim)(f, g)
#define R_30(f, g) f(30, g), R_29(f, g)
#define R_29(f, g) f(29, g), R_28(f, g)
#define R_28(f, g) f(28, g), R_27(f, g)
#define R_27(f, g) f(27, g), R_26(f, g)
#define R_26(f, g) f(26, g), R_25(f, g)
#define R_25(f, g) f(25, g), R_24(f, g)
#define R_24(f, g) f(24, g), R_23(f, g)
#define R_23(f, g) f(23, g), R_22(f, g)
#define R_22(f, g) f(22, g), R_21(f, g)
#define R_21(f, g) f(21, g), R_20(f, g)
#define R_20(f, g) f(20, g) , R_19(f, g)
#define R_19(f, g) f(19, g) , R_18(f, g)
#define R_18(f, g) f(18, g) , R_17(f, g)
#define R_17(f, g) f(17, g) , R_16(f, g)
#define R_16(f, g) f(16, g) , R_15(f, g)
#define R_15(f, g) f(15, g) , R_14(f, g)
#define R_14(f, g) f(14, g) , R_13(f, g)
#define R_13(f, g) f(13, g) , R_12(f, g)
#define R_12(f, g) f(12, g) , R_11(f, g)
#define R_11(f, g) f(11, g) , R_10(f, g)
#define R_10(f, g) f(10, g) , R_9(f, g)
#define R_9(f, g) f(9, g) , R_8(f, g)
#define R_8(f, g) f(8, g) , R_7(f, g)
#define R_7(f, g) f(7, g) , R_6(f, g)
#define R_6(f, g) f(6, g) , R_5(f, g)
#define R_5(f, g) f(5, g) , R_4(f, g)
#define R_4(f, g) f(4, g) , R_3(f, g)
#define R_3(f, g) f(3, g) , R_2(f, g)
#define R_2(f, g) f(2, g) , R_1(f, g)
#define R_1(f, g) f(1, g)
#define R_0(f, g)

#define REPEAT_NOCOMMA(f, g, lim) CONCAT(RNC_ , lim)(f, g)
#define RNC_30(f, g) f(30, g) RNC_29(f, g)
#define RNC_29(f, g) f(29, g) RNC_28(f, g)
#define RNC_28(f, g) f(28, g) RNC_27(f, g)
#define RNC_27(f, g) f(27, g) RNC_26(f, g)
#define RNC_26(f, g) f(26, g) RNC_25(f, g)
#define RNC_25(f, g) f(25, g) RNC_24(f, g)
#define RNC_24(f, g) f(24, g) RNC_23(f, g)
#define RNC_23(f, g) f(23, g) RNC_22(f, g)
#define RNC_22(f, g) f(22, g) RNC_21(f, g)
#define RNC_21(f, g) f(21, g) RNC_20(f, g)
#define RNC_20(f, g) f(20, g) RNC_19(f, g)
#define RNC_19(f, g) f(19, g) RNC_18(f, g)
#define RNC_18(f, g) f(18, g) RNC_17(f, g)
#define RNC_17(f, g) f(17, g) RNC_16(f, g)
#define RNC_16(f, g) f(16, g) RNC_15(f, g)
#define RNC_15(f, g) f(15, g) RNC_14(f, g)
#define RNC_14(f, g) f(14, g) RNC_13(f, g)
#define RNC_13(f, g) f(13, g) RNC_12(f, g)
#define RNC_12(f, g) f(12, g) RNC_11(f, g)
#define RNC_11(f, g) f(11, g) RNC_10(f, g)
#define RNC_10(f, g) f(10, g) RNC_9(f, g)
#define RNC_9(f, g) f(9, g) RNC_8(f, g)
#define RNC_8(f, g) f(8, g) RNC_7(f, g)
#define RNC_7(f, g) f(7, g) RNC_6(f, g)
#define RNC_6(f, g) f(6, g) RNC_5(f, g)
#define RNC_5(f, g) f(5, g) RNC_4(f, g)
#define RNC_4(f, g) f(4, g) RNC_3(f, g)
#define RNC_3(f, g) f(3, g) RNC_2(f, g)
#define RNC_2(f, g) f(2, g) RNC_1(f, g)
#define RNC_1(f, g) f(1, g)
#define RNC_0(f, g)

#endif

