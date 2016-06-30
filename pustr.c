#include <pustr.h>
#include <stdio.h>

#ifndef NPUSTR

size_t _pusnprintf(char *b, size_t max, const char *fmt, const pu_arg *ap){
    size_t l = 0;
    for(const char *c = fmt; *c != '\0'; c++){
        if(*c == '%'){
            const pu_arg *a = ap++;
            l += a->typed_snprint(b + l, max > l ? max - l : 0, a->val);
        }
        else if(l++ < max)
            b[l - 1] = *c;
    }
    if(l < max)
        b[l] = '\0';
    return l;
}

#define PU_DFLT_BUF_SZ 1
size_t _puprintf(const char *fmt, const pu_arg *a){
    size_t need;
    size_t max = PU_DFLT_BUF_SZ;
    for(int i = 0; i < 2; i++){
        char b[max];
        need = 1 + _pusnprintf(b, max, fmt, a);
        if(max >= need){
            fputs(b, stdout);
            break;
        }
        max = need;
    }
    return need;
}

size_t pusnprint_ptr_char(char *b, size_t l, const char **a){
    if(!*a)
        return (size_t) snprintf(b, l, "(char *)<nil>");
    return (size_t) snprintf(b, l, "%s", *a);
}

size_t pusnprint_char(char *b, size_t l, const char *a){
    if(l)
        *b = *a;
    return 1;
}

size_t pusnprint_dflt(char *b, size_t l, const void **ap){
    const size_t max = 3 + 2 * sizeof(void *);
    
    uintptr_t a = (uintptr_t) *ap;
    char tmp[max];
    char *c = &tmp[max];
    do{
        *--c = "0123456789abcdef"[a % 16];
        a /= 16;
    }while(a);

    size_t need = 3 + &tmp[max] - c;
    if(need <= l){
        b[0] = '0';
        b[1] = 'x';
        for(char *bc = &b[2]; bc != &b[need]; bc++)
            *bc = *c++;
    }

    return need - 1;
}

size_t pusnprint_uint32_t(char *b, size_t l, const uint32_t *ap){
    const size_t max = 3 * sizeof(uint32_t);

    uint32_t a = *ap;
    char tmp[max];
    char *c = &tmp[max];
    do{
        *--c = '0' + a % 10;
        a /= 10;
    }while(a);
    
    size_t need = 1 + &tmp[max] - c;
    if(need <= l){
        for(char *bc = b; bc != &b[need]; bc++)
            *bc = *c++;
    }

    return need - 1;
}

size_t pusnprint_int32_t(char *b, size_t l, const int32_t *ap){
    int32_t a = *ap;
    if(a < 0)
        a = -a;
    return pusnprint_uint32_t(b, l, (uint32_t *) &a);
}


#define pudef_dflt(t, fmt)                                          \
    size_t PU_CONCAT(pusnprint_, t)                                    \
    (char *b, size_t l, const t *a){                                \
        return (size_t) snprintf(b, l, fmt, *a);                    \
    }                                                               \
    size_t PU_CONCAT(pusnprint_ptr_, t)                              \
    (char *b, size_t l, const t **a){                               \
        if(!*a)                                                     \
            return (size_t) snprintf(b, l, "("STRLIT(t)" *)<nil>"); \
        return (size_t) snprintf(b, l, "%p:&"fmt, *a, **a);         \
    }

/* pudef_dflt(_Bool, "%"PRId8) */
/* pudef_dflt(int8_t, "%"PRId8) */
/* pudef_dflt(int16_t, "%"PRId16) */
/* pudef_dflt(int32_t, "%"PRId32) */
/* pudef_dflt(int64_t, "%"PRId64) */
/* pudef_dflt(uint8_t, "%"PRIu8) */
/* pudef_dflt(uint16_t,  "%"PRIu16) */
/* pudef_dflt(uint32_t, "%"PRIu32) */
/* pudef_dflt(uint64_t, "%"PRIu64) */
/* pudef_dflt(double, "%f") */

#endif
