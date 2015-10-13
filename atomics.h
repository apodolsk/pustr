#pragma once

uptr _xadd(iptr s, volatile uptr *p);
uptr _xchg(uptr s, volatile uptr *p);
dptr _xchg2(dptr s, volatile dptr *p);
uptr _atomic_read(volatile uptr *p);
dptr _atomic_read2(volatile dptr *p);
void _atomic_write2(dptr n, volatile dptr *p);

uptr _condxadd(uptr n, volatile uptr *p, uptr lim);
void fuzz_atomics();

uptr _cas(uptr n, volatile uptr *p, uptr old);
dptr _cas2(dptr n, volatile dptr *p, dptr old);

typedef enum{
    NOT = 0,
    WON = 1,
    OK = 2,
} howok;

howok _cas_ok(uptr n, volatile uptr *p, uptr *old);
bool _cas_won(uptr n, volatile uptr *p, uptr *old);

howok _cas2_ok(dptr n, volatile dptr *p, dptr *old);
bool _cas2_won(dptr n, volatile dptr *p, dptr *old);

howok _upd2_ok(dptr n, volatile dptr *p, dptr *old);
bool _upd2_won(dptr n, volatile dptr *p, dptr *old);


#define atomic_read(p)                          \
    PUN(typeof(*p), _atomic_read((uptr *) p))   \

#define xadd(s, d) PUN(typeof(*d),              \
                       trace(ATOMICS, 2, _xadd, s, (uptr *) d))
#define xchg(s, d) PUN(typeof(*d), _xchg(PUN(uptr, s), (uptr *) (d)))
#define xchg2(s, d) PUN(typeof(*d), _xchg2(PUN(dptr, s), (dptr *) (d)))
#define condxadd(n, d, lim)                     \
    ((typeof(*d)) _condxadd(n, (uptr *) d, (uptr) lim))

#define atomic_read2(p)                         \
    PUN(typeof(*p), _atomic_read2((dptr *) p))  \


#define atomic_write(n, p)                      \
    _atomic_write2(PUN(uptr, n), (uptr *) p)    \

#define atomic_write2(n, p)                     \
    _atomic_write2(PUN(dptr, n), (dptr *) p)    \

#define cas(n, p, old)                          \
    PUN(typeof(old),                            \
        trace(ATOMICS, 1, _cas,                 \
              PUN(uptr, n),                     \
              (uptr *) (p),                     \
              PUN(uptr, old)))

#define cas2(n, p, old)                         \
    PUN(typeof(old),                            \
        trace(ATOMICS, 1, _cas2,                \
              PUN(dptr, n),                     \
              (dptr *) (p),                     \
              PUN(dptr, old)))

#define cas_ok(n, p, op)                        \
    trace(ATOMICS, 1, _cas_ok,                  \
          PUN(uptr, n),                         \
          (uptr *) (p),                         \
          (uptr *) (op))

#define cas2_ok(n, p, op)                       \
    trace(ATOMICS, 1, _cas2_ok,                 \
          PUN(dptr, n),                         \
          (dptr *) (p),                         \
          (dptr *) (op))

#define cas_won(n, p, op)                       \
    trace(ATOMICS, 1, _cas_won,                 \
          PUN(uptr, n),                         \
          (uptr *) (p),                         \
          (uptr *) (op))

#define cas2_won(n, p, op)                      \
    trace(ATOMICS, 1, _cas2_won,                \
          PUN(dptr, n),                         \
          (dptr *) (p),                         \
          (dptr *) (op))

#define upd2_ok(n, p, op)                       \
    trace(ATOMICS, 1, _upd2_ok,                 \
          PUN(dptr, n),                         \
          (dptr *) (p),                         \
          (dptr *) (op))

#define upd2_won(n, p, op)                      \
    trace(ATOMICS, 1, _upd2_won,                \
          PUN(dptr, n),                         \
          (dptr *) (p),                         \
          (dptr *) (op))

#define pudef (howok, "%", *a == WON ? "WON" : *a == OK ? "OK" : "NOT")
#include <pudef.h>
