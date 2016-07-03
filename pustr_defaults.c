#include <pustr.h>

size_t pusnprint_char(char *b, size_t l, const char *a){
    if(l)
        *b = *a;
    return 1;
}

size_t pusnprint_ptr_char(char *b, size_t l, const char **ap){
    const char *a = *ap;
    if(!a)
        a = "<nil>";

    size_t need = 0;
    for(char c; (c = a[need]) != '\0'; need++)
        if(need < l)
            b[need] = c;
    return need;
}

__attribute__((flatten))
size_t pusnprint_dflt(char *b, size_t l, const void **ap){
    const size_t max = 2 * sizeof(void *);
    
    uintptr_t a = (uintptr_t) *ap;
    char tmp[max];
    char *c = &tmp[max];
    do{
        *--c = "0123456789abcdef"[a & 0xF];
        a >>= 4;
    }while(a);

    size_t need = 2 + &tmp[max] - c;
    if(need <= l){
        b[0] = '0';
        b[1] = 'x';
        for(char *bc = &b[2]; bc != &b[need]; bc++)
            *bc = *c++;
    }

    return need;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbool-compare"

#include <stdio.h>
#define pudef_integral(t, signed)                                       \
    size_t PU_CONCAT(pusnprint_, t)(char *b, size_t l, const t *ap){    \
        const size_t max = 3 * sizeof(t);                               \
        t a = *ap;                                                      \
        const _Bool neg = signed && a < 0;                              \
        char tmp[max];                                                  \
        char *c = &tmp[max];                                            \
        do{                                                             \
            *--c = '0' + (neg ? -(a % 10) : (a % 10));                  \
            a /= 10;                                                    \
        }while(a);                                                      \
                                                                        \
        size_t need = neg + &tmp[max] - c;                              \
        if(need <= l){                                                  \
            if(neg)                                                     \
                *b = '-';                                               \
            for(char *bc = b + neg; bc != &b[need]; bc++)               \
                *bc = *c++;                                             \
        }                                                               \
                                                                        \
        return need;                                                    \
    }                                                                   \
                                                                        \
    size_t PU_CONCAT(pusnprint_ptr_, t)(char *b, size_t l, const t **a){ \
        if(!*a)                                                         \
            return pusnprint_ptr_char(b, l,                             \
                                      (const char*[]){(char *) "<nil>"}); \
        return pusnprintf(b, l, "%:&%", (const void *) *a, **a);        \
    }

pudef_integral(int8_t, 1)
pudef_integral(int16_t, 1)
pudef_integral(int32_t, 1)
pudef_integral(int64_t, 1)
pudef_integral(_Bool, 0)
pudef_integral(uint8_t, 0)
pudef_integral(uint16_t, 0)
pudef_integral(uint32_t, 0)
pudef_integral(uint64_t, 0)
/* pudef_dflt(double, "%f") */

#include <stdio.h>
size_t pusnprint_double(char *b, size_t l, const double *d){
    return snprintf(b, l, "%f", *d);
}

#pragma GCC diagnostic pop
