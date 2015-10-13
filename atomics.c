#define MODULE ATOMICS

#include <race.h>
#include <asm.h>

#define RACE_NS 9000
#define RACE_PCNT (E_DBG_LVL ? 50 : 0)
#define RACE_MOD 2

extern uptr (xadd)(iptr s, volatile uptr *p);
extern uptr (xchg)(uptr s, volatile uptr *p);
extern uptr (cmpxchg)(uptr n, volatile uptr *p, uptr old);
extern dptr (cmpxchg2)(dptr n, volatile dptr *p, dptr old);
    
#include <time.h>
void fuzz_atomics(){
    race(RACE_NS, RACE_PCNT, RACE_MOD);
}

uptr _xadd(iptr s, volatile uptr *p){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    return (xadd)(s, p);
}

uptr _xchg(uptr s, volatile uptr *p){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    return (xchg)(s, p);
}

dptr _xchg2(dptr s, volatile dptr *p){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    while(1){
        dptr o = *p;
        if(cmpxchg2(s, p, o) == o)
            return o;
    }
}

uptr _cas(uptr n, volatile uptr *p, uptr old){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    return cmpxchg(n, p, old);
}

dptr _cas2(dptr n, volatile dptr *p, dptr old){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    return cmpxchg2(n, p, old);
}

howok _cas_ok(uptr n, volatile uptr *p, uptr *old){
    uptr o = *old;
    *old = _cas(n, p, o);
    if(*old == o)
        return WON;
    if(*old == n)
        return OK;
    return NOT;
}

howok _cas2_ok(dptr n, volatile dptr *p, dptr *old){
    dptr o = *old;
    *old = _cas2(n, p, o);
    if(*old == o)
        return WON;
    if(*old == n)
        return OK;
    return NOT;
}

bool _cas_won(uptr n, volatile uptr *p, uptr *old){
    return _cas_ok(n, p, old) == WON;
}

bool _cas2_won(dptr n, volatile dptr *p, dptr *old){
    return _cas2_ok(n, p, old) == WON;
}

howok _upd2_ok(dptr n, volatile dptr *p, dptr *old){
    howok r = _cas2_ok(n, p, old) == WON;
    if(r == WON)
        *old = n;
    return r;
}

bool _upd2_won(dptr n, volatile dptr *p, dptr *old){
    return _upd2_ok(n, p, old) == WON;
}

uptr _atomic_read(volatile uptr *p){
    fuzz_atomics();
    return *p;
}

void _atomic_write(uptr n, volatile uptr *p){
    fuzz_atomics();
    *p = n;
}

dptr _atomic_read2(volatile dptr *p){
    return _cas2(0, p, 0);
}

void _atomic_write2(dptr n, volatile dptr *p){
    for(dptr o = *p;;)
        if(_cas2_won(n, p, &o))
            return;
}

uptr _condxadd(uptr n, volatile uptr *p, uptr lim){
    for(uptr r = *p;;){
        assert(r < lim);
        if(r + n >= lim)
            return lim;
        if(_cas_won(r + n, p, &r))
            return r;
    }
}
