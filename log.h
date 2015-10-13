#pragma once

#include <pustr.h>
#include <logverb.h>

#define lprintf(fmt, as...) puprintf("% "fmt"\n", get_dbg_id(), ##as)

#define NAMEFMT(a, _, __) a:%

#if !LOG_MASTER
#define log(...) 0
#define pp(as...) (as)
#define ppl(lvl, as...) (as)
#define trace(module, lvl, f, as...) f(as)
#else

#define ppl(lvl, as...)                                                 \
    log_cond(lvl, MODULE,                                               \
             ({                                                         \
                 MAP_NOCOMMA(estore, __pp, as);                         \
                 lprintf(STRLIT(MAP(NAMEFMT,, ##as)), MAP(eref, __pp, as)); \
                 MAP(eref, __pp, as);                                   \
             }),                                                        \
             as)
#define pp(as...) ppl(1, ##as)

#define log(lvl, fmt, as...) log_cond(lvl, MODULE, lprintf(fmt, ##as), 0)

#define trace(module, lvl, f, as...)            \
    log_cond(lvl, module, putrace(lprintf, f, ##as), f(as))
#endif

#define log_cond(lvl, module, e, or...)                             \
    CONCAT(log_cond, CONCAT2(CONCAT3(LOG_, module), lvl) ) (e, or)

/* The point of this is to cut down on compilation time. Consider the
   alternative of generating complicated _Generic() expressions that'll
   have to propogate to some DCE phase. */
#define log_cond00(e, or...) e
#define log_cond01(e, or...) or
#define log_cond02(e, or...) or
#define log_cond03(e, or...) or
#define log_cond04(e, or...) or
#define log_cond05(e, or...) or
#define log_cond10(e, or...) e
#define log_cond11(e, or...) e
#define log_cond12(e, or...) or
#define log_cond13(e, or...) or
#define log_cond14(e, or...) or
#define log_cond15(e, or...) or
#define log_cond20(e, or...) e
#define log_cond21(e, or...) e
#define log_cond22(e, or...) e
#define log_cond23(e, or...) or
#define log_cond24(e, or...) or
#define log_cond25(e, or...) or
#define log_cond30(e, or...) e
#define log_cond31(e, or...) e
#define log_cond32(e, or...) e
#define log_cond33(e, or...) e
#define log_cond34(e, or...) or
#define log_cond35(e, or...) or
#define log_cond40(e, or...) e
#define log_cond41(e, or...) e
#define log_cond42(e, or...) e
#define log_cond43(e, or...) e
#define log_cond44(e, or...) e
#define log_cond45(e, or...) or
#define log_cond50(e, or...) e
#define log_cond51(e, or...) e
#define log_cond52(e, or...) e
#define log_cond53(e, or...) e
#define log_cond54(e, or...) e
#define log_cond55(e, or...) e
