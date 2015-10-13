#include <pustr.h>
#include <stdio.h>

size_t puvsnprintf(char *b, size_t max, const char *fmt, va_list args){
    size_t l = 0;
    for(const char *c = fmt; *c != '\0'; c++){
        if(*c == '%'){
            pu_arg *a = va_arg(args, pu_arg *);
            assert(a && a->typed_snprint);
            l += a->typed_snprint(b + l, max > l ? max - l : 0, a->val);
        }
        else if(l++ < max)
            b[l - 1] = *c;
    }
    if(l < max)
        b[l] = '\0';
    return l;
}

size_t _pusnprintf(char *b, size_t max, const char *fmt, ...){
    va_list args; 
    va_start(args, fmt);
    size_t l = puvsnprintf(b, max, fmt, args);
    va_end(args);
    return l;
}

#define PU_DFLT_BUF_SZ 80
size_t _puprintf(const char *fmt, ...){
    size_t max = PU_DFLT_BUF_SZ, need;
    for(int i = 0; i < 2; i++){
        char b[max];
        va_list l;
        va_start(l, fmt);
        need = 1 + puvsnprintf(b, max, fmt, l);
        va_end(l);
        if(max >= need){
            fputs(b, stdout);
            break;
        }
        max = need;
    }
    return need;
}

size_t pusnprint_ptr_char(char *b, size_t l, volatile const char **a){
    if(!*a)
        return (size_t) snprintf(b, l, "(char *)<nil>");
    return (size_t) snprintf(b, l, "%s", *a);
}

size_t pusnprint_char(char *b, size_t l, volatile const char *a){
    if(l)
        *b = *a;
    return 1;
}

size_t pusnprint_dflt(char *b, size_t l, volatile const void **a){
    return (size_t) snprintf(b, l, "%p", *a);
}

#define pudef_dflt(t, fmt)                                              \
    size_t CONCAT(pusnprint_, t)                                        \
    (char *b, size_t l, volatile const t *a){                           \
        return (size_t) snprintf(b, l, fmt, *a);                         \
    }                                                                   \
    size_t CONCAT(pusnprint_ptr_, t)                                    \
    (char *b, size_t l, volatile const t **a){                          \
        if(!*a)                                                         \
            return (size_t) snprintf(b, l, "("STRLIT(t)" *)<nil>");      \
        return (size_t) snprintf(b, l, "%p:&"fmt, *a, **a);              \
    }

pudef_dflt(bool, "%"PRId8)
pudef_dflt(int8_t, "%"PRId8)
pudef_dflt(int16_t, "%"PRId16)
pudef_dflt(int32_t, "%"PRId32)
pudef_dflt(int64_t, "%"PRId64)
pudef_dflt(uint8_t, "%"PRIu8)
pudef_dflt(uint16_t,  "%"PRIu16)
pudef_dflt(uint32_t, "%"PRIu32)
pudef_dflt(uint64_t, "%"PRIu64)
pudef_dflt(double, "%f")
