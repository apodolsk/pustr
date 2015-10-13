#pragma once

/* Turn array expr into pointer expr. */
#define decay(e) ((void) 0, e)

/* Create name-mangled variables to avoid double-eval of expr. */
#define estore(e, pfx, i) typeof(decay(e)) CONCAT(pfx, i) = e;
#define eref(_, pfx, i) CONCAT(pfx, i)

#define STRLIT(xs...) _STRLIT(xs)
#define _STRLIT(xs...) #xs

#define CONCAT(x, y) _CONCAT(x, y)
#define _CONCAT(x, y) x##y

#define CONCAT2(x, y) _CONCAT2(x, y)
#define _CONCAT2(x, y) x##y

#define CONCAT3(x, y) _CONCAT3(x, y)
#define _CONCAT3(x, y) x##y

#define ISEMPTY(as...)  _ISEMPTY(_, ##as, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
#define _ISEMPTY(_, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) a11

/* Useful for de-tupling. */
#define COMMAPFX_IF_NZ(a...) _COMMAPFX_IF_NZ(a)
#define _COMMAPFX_IF_NZ(a...) , ##a


/** 
 * @brief Expands to the number of args which it has been passed, or to 0
 * if it's passed a single argument of zero preprocessing tokens ("zero"
 * args).
 *
 * Danger: As an arbitrary limit, it's undefined for more than 40 args.
 * 
 * Idea: Take a macro that returns its n'th argument and swallows the
 * rest, and then construct a call to it so that the n'th argument happens
 * to be equal to the number of arguments which it was passed. As the
 * number of args in __VA_ARGS__ in _NUM_ARGS increases, the smaller
 * numbers in the hardcoded count are swallowed up.
 *
 * The use of ##__VA_ARGS__ combined with a dummy arg is to distinguish
 * between one empty arg ("zero args") and one non-empty arg, which the
 * GCC comma-swallowing extension is capable of doing. _NUM_ARGS returns
 * the number of args it's passed minus one, and it's passed one arg when
 * __VA_ARGS__ is empty and NUM_ARGS(__VA_ARGS__) - 1 otherwise.
 *
 * TODO: remember why the indirection matters. HINT: constructing
 * invocations from tuples. It's probably not just the ##.
 */
#define NUM_ARGS(as...) _NUM_ARGS(as)
#define _NUM_ARGS(...)                                                  \
    GET_41ST(                                                           \
        _, ##__VA_ARGS__, 39, 38, 37, 36, 35,                           \
        34, 33, 32, 31, 30, 29, 28, 27, 26, 25,                         \
        24, 23, 22, 21, 20, 19, 18, 17, 16, 15,                         \
        14, 13, 12, 11, 10, 9, 8, 7, 6, 5,                              \
        4, 3, 2, 1, 0)                                                  \
    
#define GET_41ST(a1, a2, a3, a4, a5, a6, a7,                        \
                 a8, a9, a10, a11, a12, a13,                        \
                 a14, a15, a16, a17, a18, a19, a20,                 \
                 a21, a22, a23, a24, a25, a26, a27,                 \
                 a28, a29, a30, a31, a32, a33, a34,                 \
                 a35, a36, a37, a38, a39, a40,                      \
                 N, ...)                                            \
    N

/** 
 * Invoke macro FUNC on each arg, up to 20 args. Each invocation is passed
 * the global argument and the index of its argument, where indexing
 * confusingly starts from 0 at the END of the argument list.
 *
 * Idea: hardcode an iterative nested macro expansion, and use NUM_ARGS +
 * CONCAT to decide what depth of this iteration to jump into for a given
 * invocation.
 */
#define MAP(FUNC, global, ...) CONCAT(MAP_ , NUM_ARGS(__VA_ARGS__)) \
    (FUNC, global, __VA_ARGS__)

#define MAP_20(f, g, arg, ...) f(arg, g, 19), MAP_19(f, g, __VA_ARGS__)
#define MAP_19(f, g, arg, ...) f(arg, g, 18), MAP_18(f, g, __VA_ARGS__)
#define MAP_18(f, g, arg, ...) f(arg, g, 17), MAP_17(f, g, __VA_ARGS__)
#define MAP_17(f, g, arg, ...) f(arg, g, 16), MAP_16(f, g, __VA_ARGS__)
#define MAP_16(f, g, arg, ...) f(arg, g, 15), MAP_15(f, g, __VA_ARGS__)
#define MAP_15(f, g, arg, ...) f(arg, g, 14), MAP_14(f, g, __VA_ARGS__)
#define MAP_14(f, g, arg, ...) f(arg, g, 13), MAP_13(f, g, __VA_ARGS__)
#define MAP_13(f, g, arg, ...) f(arg, g, 12), MAP_12(f, g, __VA_ARGS__)
#define MAP_12(f, g, arg, ...) f(arg, g, 11), MAP_11(f, g, __VA_ARGS__)
#define MAP_11(f, g, arg, ...) f(arg, g, 10), MAP_10(f, g, __VA_ARGS__)
#define MAP_10(f, g, arg, ...) f(arg, g, 9), MAP_9(f, g, __VA_ARGS__)
#define MAP_9(f, g, arg, ...) f(arg, g, 8), MAP_8(f, g, __VA_ARGS__)
#define MAP_8(f, g, arg, ...) f(arg, g, 7), MAP_7(f, g, __VA_ARGS__)
#define MAP_7(f, g, arg, ...) f(arg, g, 6), MAP_6(f, g, __VA_ARGS__)
#define MAP_6(f, g, arg, ...) f(arg, g, 5), MAP_5(f, g, __VA_ARGS__)
#define MAP_5(f, g, arg, ...) f(arg, g, 4), MAP_4(f, g, __VA_ARGS__)
#define MAP_4(f, g, arg, ...) f(arg, g, 3), MAP_3(f, g, __VA_ARGS__)
#define MAP_3(f, g, arg, ...) f(arg, g, 2), MAP_2(f, g, __VA_ARGS__)
#define MAP_2(f, g, arg, ...) f(arg, g, 1), MAP_1(f, g, __VA_ARGS__)
#define MAP_1(f, g, arg) f(arg, g, 0)
#define MAP_0(f, g, arg)

/* This is here because MAP() can't be used inside an expansion of
   MAP() - ie the preprocessor doesn't allow recursion. */
#define MAP2(FUNC, global, ...) CONCAT(MAP2_ , NUM_ARGS(__VA_ARGS__)) \
    (FUNC, global, __VA_ARGS__)

#define MAP2_20(f, g, arg, ...) f(arg, g, 19), MAP2_19(f, g, __VA_ARGS__)
#define MAP2_19(f, g, arg, ...) f(arg, g, 18), MAP2_18(f, g, __VA_ARGS__)
#define MAP2_18(f, g, arg, ...) f(arg, g, 17), MAP2_17(f, g, __VA_ARGS__)
#define MAP2_17(f, g, arg, ...) f(arg, g, 16), MAP2_16(f, g, __VA_ARGS__)
#define MAP2_16(f, g, arg, ...) f(arg, g, 15), MAP2_15(f, g, __VA_ARGS__)
#define MAP2_15(f, g, arg, ...) f(arg, g, 14), MAP2_14(f, g, __VA_ARGS__)
#define MAP2_14(f, g, arg, ...) f(arg, g, 13), MAP2_13(f, g, __VA_ARGS__)
#define MAP2_13(f, g, arg, ...) f(arg, g, 12), MAP2_12(f, g, __VA_ARGS__)
#define MAP2_12(f, g, arg, ...) f(arg, g, 11), MAP2_11(f, g, __VA_ARGS__)
#define MAP2_11(f, g, arg, ...) f(arg, g, 10), MAP2_10(f, g, __VA_ARGS__)
#define MAP2_10(f, g, arg, ...) f(arg, g, 9), MAP2_9(f, g, __VA_ARGS__)
#define MAP2_9(f, g, arg, ...) f(arg, g, 8), MAP2_8(f, g, __VA_ARGS__)
#define MAP2_8(f, g, arg, ...) f(arg, g, 7), MAP2_7(f, g, __VA_ARGS__)
#define MAP2_7(f, g, arg, ...) f(arg, g, 6), MAP2_6(f, g, __VA_ARGS__)
#define MAP2_6(f, g, arg, ...) f(arg, g, 5), MAP2_5(f, g, __VA_ARGS__)
#define MAP2_5(f, g, arg, ...) f(arg, g, 4), MAP2_4(f, g, __VA_ARGS__)
#define MAP2_4(f, g, arg, ...) f(arg, g, 3), MAP2_3(f, g, __VA_ARGS__)
#define MAP2_3(f, g, arg, ...) f(arg, g, 2), MAP2_2(f, g, __VA_ARGS__)
#define MAP2_2(f, g, arg, ...) f(arg, g, 1), MAP2_1(f, g, __VA_ARGS__)
#define MAP2_1(f, g, arg) f(arg, g, 0)
#define MAP2_0(f, g, arg)

#define MAP3(FUNC, global, ...) CONCAT(MAP3_ , NUM_ARGS(__VA_ARGS__)) \
    (FUNC, global, __VA_ARGS__)

#define MAP3_20(f, g, arg, ...) f(arg, g, 19), MAP3_19(f, g, __VA_ARGS__)
#define MAP3_19(f, g, arg, ...) f(arg, g, 18), MAP3_18(f, g, __VA_ARGS__)
#define MAP3_18(f, g, arg, ...) f(arg, g, 17), MAP3_17(f, g, __VA_ARGS__)
#define MAP3_17(f, g, arg, ...) f(arg, g, 16), MAP3_16(f, g, __VA_ARGS__)
#define MAP3_16(f, g, arg, ...) f(arg, g, 15), MAP3_15(f, g, __VA_ARGS__)
#define MAP3_15(f, g, arg, ...) f(arg, g, 14), MAP3_14(f, g, __VA_ARGS__)
#define MAP3_14(f, g, arg, ...) f(arg, g, 13), MAP3_13(f, g, __VA_ARGS__)
#define MAP3_13(f, g, arg, ...) f(arg, g, 12), MAP3_12(f, g, __VA_ARGS__)
#define MAP3_12(f, g, arg, ...) f(arg, g, 11), MAP3_11(f, g, __VA_ARGS__)
#define MAP3_11(f, g, arg, ...) f(arg, g, 10), MAP3_10(f, g, __VA_ARGS__)
#define MAP3_10(f, g, arg, ...) f(arg, g, 9), MAP3_9(f, g, __VA_ARGS__)
#define MAP3_9(f, g, arg, ...) f(arg, g, 8), MAP3_8(f, g, __VA_ARGS__)
#define MAP3_8(f, g, arg, ...) f(arg, g, 7), MAP3_7(f, g, __VA_ARGS__)
#define MAP3_7(f, g, arg, ...) f(arg, g, 6), MAP3_6(f, g, __VA_ARGS__)
#define MAP3_6(f, g, arg, ...) f(arg, g, 5), MAP3_5(f, g, __VA_ARGS__)
#define MAP3_5(f, g, arg, ...) f(arg, g, 4), MAP3_4(f, g, __VA_ARGS__)
#define MAP3_4(f, g, arg, ...) f(arg, g, 3), MAP3_3(f, g, __VA_ARGS__)
#define MAP3_3(f, g, arg, ...) f(arg, g, 2), MAP3_2(f, g, __VA_ARGS__)
#define MAP3_2(f, g, arg, ...) f(arg, g, 1), MAP3_1(f, g, __VA_ARGS__)
#define MAP3_1(f, g, arg) f(arg, g, 0)
#define MAP3_0(f, g, arg)


#define MAP_NOCOMMA(FUNC, global, ...)          \
    CONCAT(MAPNC_ , NUM_ARGS(__VA_ARGS__))(FUNC, global, __VA_ARGS__)

#define MAPNC_20(f, g, arg, ...) f(arg, g, 19) MAPNC_19(f, g, __VA_ARGS__)
#define MAPNC_19(f, g, arg, ...) f(arg, g, 18) MAPNC_18(f, g, __VA_ARGS__)
#define MAPNC_18(f, g, arg, ...) f(arg, g, 17) MAPNC_17(f, g, __VA_ARGS__)
#define MAPNC_17(f, g, arg, ...) f(arg, g, 16) MAPNC_16(f, g, __VA_ARGS__)
#define MAPNC_16(f, g, arg, ...) f(arg, g, 15) MAPNC_15(f, g, __VA_ARGS__)
#define MAPNC_15(f, g, arg, ...) f(arg, g, 14) MAPNC_14(f, g, __VA_ARGS__)
#define MAPNC_14(f, g, arg, ...) f(arg, g, 13) MAPNC_13(f, g, __VA_ARGS__)
#define MAPNC_13(f, g, arg, ...) f(arg, g, 12) MAPNC_12(f, g, __VA_ARGS__)
#define MAPNC_12(f, g, arg, ...) f(arg, g, 11) MAPNC_11(f, g, __VA_ARGS__)
#define MAPNC_11(f, g, arg, ...) f(arg, g, 10) MAPNC_10(f, g, __VA_ARGS__)
#define MAPNC_10(f, g, arg, ...) f(arg, g, 9) MAPNC_9(f, g, __VA_ARGS__)
#define MAPNC_9(f, g, arg, ...) f(arg, g, 8) MAPNC_8(f, g, __VA_ARGS__)
#define MAPNC_8(f, g, arg, ...) f(arg, g, 7) MAPNC_7(f, g, __VA_ARGS__)
#define MAPNC_7(f, g, arg, ...) f(arg, g, 6) MAPNC_6(f, g, __VA_ARGS__)
#define MAPNC_6(f, g, arg, ...) f(arg, g, 5) MAPNC_5(f, g, __VA_ARGS__)
#define MAPNC_5(f, g, arg, ...) f(arg, g, 4) MAPNC_4(f, g, __VA_ARGS__)
#define MAPNC_4(f, g, arg, ...) f(arg, g, 3) MAPNC_3(f, g, __VA_ARGS__)
#define MAPNC_3(f, g, arg, ...) f(arg, g, 2) MAPNC_2(f, g, __VA_ARGS__)
#define MAPNC_2(f, g, arg, ...) f(arg, g, 1) MAPNC_1(f, g, __VA_ARGS__)
#define MAPNC_1(f, g, arg) f(arg, g, 0)
#define MAPNC_0(f, g, arg)

#define ITERATE(f, g, lim) CONCAT(I_ , lim)(f, g, lim)
#define I_20(f, g, lim) f(lim-20, g) , I_19(f, g, lim)
#define I_19(f, g, lim) f(lim-19, g) , I_18(f, g, lim)
#define I_18(f, g, lim) f(lim-18, g) , I_17(f, g, lim)
#define I_17(f, g, lim) f(lim-17, g) , I_16(f, g, lim)
#define I_16(f, g, lim) f(lim-16, g) , I_15(f, g, lim)
#define I_15(f, g, lim) f(lim-15, g) , I_14(f, g, lim)
#define I_14(f, g, lim) f(lim-14, g) , I_13(f, g, lim)
#define I_13(f, g, lim) f(lim-13, g) , I_12(f, g, lim)
#define I_12(f, g, lim) f(lim-12, g) , I_11(f, g, lim)
#define I_11(f, g, lim) f(lim-11, g) , I_10(f, g, lim)
#define I_10(f, g, lim) f(lim-10, g) , I_9(f, g, lim)
#define I_9(f, g, lim) f(lim-9, g) , I_8(f, g, lim)
#define I_8(f, g, lim) f(lim-8, g) , I_7(f, g, lim)
#define I_7(f, g, lim) f(lim-7, g) , I_6(f, g, lim)
#define I_6(f, g, lim) f(lim-6, g) , I_5(f, g, lim)
#define I_5(f, g, lim) f(lim-5, g) , I_4(f, g, lim)
#define I_4(f, g, lim) f(lim-4, g) , I_3(f, g, lim)
#define I_3(f, g, lim) f(lim-3, g) , I_2(f, g, lim)
#define I_2(f, g, lim) f(lim-2, g) , I_1(f, g, lim)
#define I_1(f, g, lim) f(lim-1, g)
#define I_0(f, g, lim)

#define ITERATE_NOCOMMA(f, g, lim) CONCAT(INC_ , lim)(f, g, lim)
#define INC_20(f, g, lim) f(lim-20, g) INC_19(f, g, lim)
#define INC_19(f, g, lim) f(lim-19, g) INC_18(f, g, lim)
#define INC_18(f, g, lim) f(lim-18, g) INC_17(f, g, lim)
#define INC_17(f, g, lim) f(lim-17, g) INC_16(f, g, lim)
#define INC_16(f, g, lim) f(lim-16, g) INC_15(f, g, lim)
#define INC_15(f, g, lim) f(lim-15, g) INC_14(f, g, lim)
#define INC_14(f, g, lim) f(lim-14, g) INC_13(f, g, lim)
#define INC_13(f, g, lim) f(lim-13, g) INC_12(f, g, lim)
#define INC_12(f, g, lim) f(lim-12, g) INC_11(f, g, lim)
#define INC_11(f, g, lim) f(lim-11, g) INC_10(f, g, lim)
#define INC_10(f, g, lim) f(lim-10, g) INC_9(f, g, lim)
#define INC_9(f, g, lim) f(lim-9, g) INC_8(f, g, lim)
#define INC_8(f, g, lim) f(lim-8, g) INC_7(f, g, lim)
#define INC_7(f, g, lim) f(lim-7, g) INC_6(f, g, lim)
#define INC_6(f, g, lim) f(lim-6, g) INC_5(f, g, lim)
#define INC_5(f, g, lim) f(lim-5, g) INC_4(f, g, lim)
#define INC_4(f, g, lim) f(lim-4, g) INC_3(f, g, lim)
#define INC_3(f, g, lim) f(lim-3, g) INC_2(f, g, lim)
#define INC_2(f, g, lim) f(lim-2, g) INC_1(f, g, lim)
#define INC_1(f, g, lim) f(lim-1, g)
#define INC_0(f, g, lim)

#define ISZERO(i) CONCAT(ISZERO, i)
#define ISZERO0 1
#define ISZERO1 0
#define ISZERO2 0
#define ISZERO3 0
#define ISZERO4 0
#define ISZERO5 0
#define ISZERO6 0
#define ISZERO7 0
#define ISZERO8 0
#define ISZERO9 0
#define ISZERO10 0

