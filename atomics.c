#define MODULE ATOMICS
#undef E_ATOMICS
#define E_ATOMICS 0, LVL_TODO, LVL_TODO

#include <race.h>
#include <asm.h>

#define RACE_NS 900
#define RACE_PCNT (E_DBG_LVL ? 20 : 0)
#define RACE_MOD 1

u32 __sync_fetch_and_add_4(volatile u32 *p, u32 a){
    asm volatile("lock xadd %0, %1"
                 :"+r" (a), "+m" (*p));
    return a;
}

/* TODO: pretty much a mess. */
u32 __sync_val_compare_and_swap_4(volatile u32 *p, u32 old, u32 n){
    asm volatile("lock cmpxchgl %2, %1"
                 :"+a" (old), "+m" (*p)
                 :"r" (n)
                 :"cc", "memory");
    return old;
}

i64 __sync_val_compare_and_swap_8(volatile i64 *p, i64 old, i64 n){
    union {
        struct{
            u32 lo;
            u32 hi;
        };
        u64 i;
    } _n = {.i = n};
    asm volatile("lock cmpxchg8b %1"
                 :"+A" (old), "+m" (*p)
                 :"c" (_n.hi), "b" (_n.lo)
                 :"cc", "memory");
    return old;
}

static
bool _atomic_compare_exchange_4(volatile u32 *p, u32 *old, u32 n){
    bool r;
    asm volatile("lock cmpxchgl %3, %1"
                 :"+a" (*old), "+m" (*p), "=@ccz" (r)
                 :"r" (n)
                 : "memory");
    return r;
}

static
bool _atomic_compare_exchange_8(volatile i64 *p, i64 *old, i64 n){
    bool r;
    union {
        struct{
            u32 lo;
            u32 hi;
        };
        u64 i;
    } _n = {.i = n};
    asm volatile("lock cmpxchg8b %1"
                 :"+A" (*old), "+m" (*p), "=@ccz" (r)
                 :"c" (_n.hi), "b" (_n.lo)
                 :"memory");
    return r;
}
                      
#include <time.h>
void fuzz_atomics(){
    race(RACE_NS, RACE_PCNT, RACE_MOD);
}

uptr _xadd(iptr s, volatile uptr *p){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    return __sync_fetch_and_add(p, s, __ATOMIC_SEQ_CST);
}

uptr _xchg(uptr s, volatile uptr *p){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    return __atomic_exchange_n(p, s, __ATOMIC_SEQ_CST);
}

dptr _xchg2(dptr s, volatile dptr *p){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    for(dptr o = *p;;)
        if(_atomic_compare_exchange_8(p, &o, s))
            return o;
}


uptr _cas(uptr n, volatile uptr *p, uptr old){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    return __sync_val_compare_and_swap(p, old, n);
}

dptr _cas2(dptr n, volatile dptr *p, dptr old){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    return __sync_val_compare_and_swap(p, old, n);
}

bool _cas_won(uptr n, volatile uptr *p, uptr *old){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    return _atomic_compare_exchange_4(p, old, n);
}


bool _cas2_won(dptr n, volatile dptr *p, dptr *old){
    assert(aligned_pow2(p, sizeof(*p)));
    fuzz_atomics();
    return _atomic_compare_exchange_8(p, old, n);
}

howok _cas_ok(uptr n, volatile uptr *p, uptr *old){
    if(_cas_won(n, p, old))
        return WON;
    if(*old == n)
        return OK;
    return NOT;
}

howok _cas2_ok(dptr n, volatile dptr *p, dptr *old){
    if(_cas2_won(n, p, old))
        return WON;
    if(*old == n)
        return OK;
    return NOT;
}

bool _upd_won(uptr n, volatile uptr *p, uptr *old){
    if(!_cas_won(n, p, old))
        return false;
    *old = n;
    return true;
}

bool _upd2_won(dptr n, volatile dptr *p, dptr *old){
    if(!_cas2_won(n, p, old))
        return false;
    *old = n;
    return true;
}

howok _upd_ok(uptr n, volatile uptr *p, uptr *old){
    if(_upd_won(n, p, old))
        return WON;
    if(*old == n)
        return OK;
    return NOT;
}

howok _upd2_ok(dptr n, volatile dptr *p, dptr *old){
    if(_upd2_won(n, p, old))
        return WON;
    if(*old == n)
        return OK;
    return NOT;
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

uptr _xadd_iff(uptr a, volatile uptr *p, uptr lim){
    for(uptr r = *p;;){
        if(r + a > lim)
            return r;
        if(_cas_won(r + a, p, &r))
            return r;
    }
}

uptr _xsub_iff(uptr s, volatile uptr *p){
    for(uptr r = *p;;)
        if(r < s || _cas_won(r - s, p, &r))
            return r;
}
