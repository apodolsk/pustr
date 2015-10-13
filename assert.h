#pragma once
#include <errors.h>

#define assert(expr...)                                              \
    ass_cond(1, E_DBG_LVL,                                           \
             (!(expr) ? TODO("Failed assertion: %.", #expr), 1 : 1), \
             0 ? expr : 0)

#define assertl(req_dbg_lvl, expr...)                                \
    ass_cond(req_dbg_lvl, E_DBG_LVL,                                 \
             (!(expr) ? TODO("Failed assertion: %.", #expr), 1 : 1), \
             0 ? expr : 0)

#define ass_cond(req, verb, e, or...)               \
    CONCAT(ass_cond, CONCAT2(verb, req)) (e, or)

#define ass_cond00(e, or...) e
#define ass_cond01(e, or...) or
#define ass_cond02(e, or...) or
#define ass_cond03(e, or...) or
#define ass_cond04(e, or...) or
#define ass_cond05(e, or...) or
#define ass_cond10(e, or...) e
#define ass_cond11(e, or...) e
#define ass_cond12(e, or...) or
#define ass_cond13(e, or...) or
#define ass_cond14(e, or...) or
#define ass_cond15(e, or...) or
#define ass_cond20(e, or...) e
#define ass_cond21(e, or...) e
#define ass_cond22(e, or...) e
#define ass_cond23(e, or...) or
#define ass_cond24(e, or...) or
#define ass_cond25(e, or...) or
#define ass_cond30(e, or...) e
#define ass_cond31(e, or...) e
#define ass_cond32(e, or...) e
#define ass_cond33(e, or...) e
#define ass_cond34(e, or...) or
#define ass_cond35(e, or...) or
#define ass_cond40(e, or...) e
#define ass_cond41(e, or...) e
#define ass_cond42(e, or...) e
#define ass_cond43(e, or...) e
#define ass_cond44(e, or...) e
#define ass_cond45(e, or...) or
#define ass_cond50(e, or...) e
#define ass_cond51(e, or...) e
#define ass_cond52(e, or...) e
#define ass_cond53(e, or...) e
#define ass_cond54(e, or...) e
#define ass_cond55(e, or...) e
