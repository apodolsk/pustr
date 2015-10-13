#pragma once
#include <dial_macros.h>
#include <config.h>
#include <errverbs.h>

/* Default break, print, and dbg levels. */

#define LVL_EWTF 0
#define LVL_TODO 1
#define LVL_SUPER_EARG 1
#define LVL_OVERCOMMIT 2
#define LVL_SUPER_RARITY 2
#define LVL_EOOR 3
#define LVL_EARG 4
#define LVL_RARITY 5

#define FIRST(as...) _FIRST(as)
#define SECOND(as...) _SECOND(as)
#define THIRD(as...) _THIRD(as)
#define _FIRST(a, ...) a
#define _SECOND(a, b, ...) b
#define _THIRD(a, b, c) c

#define LOOKUP _LOOKUP(CONCAT(E_, MODULE))
#define _LOOKUP(mod) CONCAT2(_LOOKUP, NUM_ARGS(mod))(mod)
#define _LOOKUP1(mod) DBG, BRK, PRNT
#define _LOOKUP3(a, b, c) a, b, c

#define E_DBG_LVL FIRST(LOOKUP)
#define E_BREAK_LVL SECOND(LOOKUP)
#define E_PRINT_LVL THIRD(LOOKUP)

extern noreturn void panic(const char *, ...);

/* --- Fatal Errors (for the kernel) --- */

#define EWTF(fmt, as...)                                \
    ({                                                  \
        elog(0, "This can't be. %:%:%. " fmt    \
             , __FILE__ ,__func__, __LINE__, ##as);     \
        ebreakpoint(0);                                      \
        panic("WTF!");                                  \
    })

#define TODO(fmt, as...)                                            \
    ({                                                              \
        elog(0, "My creator has abandoned me. %:%:%. " fmt  \
             , __FILE__ , __func__ , __LINE__, ##as);               \
        ebreakpoint(0);                                             \
        panic("TODO!");                                             \
    })                                            


/* --- Recoverable Errors (for the kernel) --- */

/* Sequel to EARG on the NES. */
#define SUPER_EARG(fmt, as...)                          \
    ({                                                  \
        elog(1, "Super bad input error. %:%:%. "fmt,    \
             __FILE__, __func__, __LINE__, ##as);       \
        ebreakpoint(1);                                 \
        (dptr) -1;                                       \
    })

#define OVERCOMMIT_ERROR(fmt, as...)                    \
    ({                                                  \
        elog(2,"Overcommit error. %:%:%. "      \
             fmt, __FILE__, __func__, __LINE__, ##as);  \
        ebreakpoint(2);                                      \
        (dptr) -1;                                            \
    })

#define SUPER_RARITY(fmt, as...)                        \
    ({                                                  \
        elog(2,"Super rare event. %:%:%. "              \
             fmt, __FILE__, __func__, __LINE__, ##as);  \
        ebreakpoint(2);                                 \
        (dptr) -1;                                      \
    })                                                  \

#define EOOR(fmt, as...)                            \
    ({                                              \
        elog(3,"Out of resources. %:%:%. " fmt      \
             , __FILE__, __func__, __LINE__, ##as); \
        ebreakpoint(3);                             \
        (dptr) EOOR;                                \
    })                                                         

        
#define EARG(fmt, as...)                            \
    ({                                              \
        elog(4, "Input error. %:%:%. " fmt          \
             , __FILE__, __func__, __LINE__, ##as); \
        ebreakpoint(4);                             \
        (dptr) EARG;                                \
    })

#define RARITY(fmt, as...)                              \
    ({                                                  \
        elog(5, "Rarity. %:%:%. "               \
             fmt, __FILE__, __func__, __LINE__, ##as);  \
        ebreakpoint(5);                                      \
    })                                                  \

/* --- Helpers --- */

#define elog(lvl, fmt, ...)                                     \
    err_cond(lvl, E_PRINT_LVL, lprintf(fmt, ##__VA_ARGS__), 0)

#define ebreakpoint(lvl)                        \
    err_cond(lvl, E_BREAK_LVL, breakpoint(), 0)

/* TODO: macro-expansion into multiple macro args not happening. Same old
   shit. */
#define KERNPTR_MSG(addr)                                   \
    /* "Forbidden pointer to kern memory: %", (void *) addr */

#define BADWRITE_MSG(addr)                      \
    /* "Failed to write to %", (void *) addr */

#define BADREAD_MSG(addr)                       \
    /* "Failed to read from: %", (void *) addr      */

#define BADMEM_MSG(addr)                                     \
    /* "Unreadable or unwriteable memory: %.", (void *) addr    */

#define err_cond(req, verb, e, or...)                   \
    CONCAT(log_cond, CONCAT2(verb, req)) (e, or)

#define err_cond00(e, or...) e
#define err_cond01(e, or...) or
#define err_cond02(e, or...) or
#define err_cond03(e, or...) or
#define err_cond04(e, or...) or
#define err_cond05(e, or...) or
#define err_cond10(e, or...) e
#define err_cond11(e, or...) e
#define err_cond12(e, or...) or
#define err_cond13(e, or...) or
#define err_cond14(e, or...) or
#define err_cond15(e, or...) or
#define err_cond20(e, or...) e
#define err_cond21(e, or...) e
#define err_cond22(e, or...) e
#define err_cond23(e, or...) or
#define err_cond24(e, or...) or
#define err_cond25(e, or...) or
#define err_cond30(e, or...) e
#define err_cond31(e, or...) e
#define err_cond32(e, or...) e
#define err_cond33(e, or...) e
#define err_cond34(e, or...) or
#define err_cond35(e, or...) or
#define err_cond40(e, or...) e
#define err_cond41(e, or...) e
#define err_cond42(e, or...) e
#define err_cond43(e, or...) e
#define err_cond44(e, or...) e
#define err_cond45(e, or...) or
#define err_cond50(e, or...) e
#define err_cond51(e, or...) e
#define err_cond52(e, or...) e
#define err_cond53(e, or...) e
#define err_cond54(e, or...) e
#define err_cond55(e, or...) e

