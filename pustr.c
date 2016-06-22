#include <pustr.h>
#include <stdio.h>

#ifndef NPUSTR

static
size_t __pusnprintf(char *b, size_t max, const char *fmt, const pu_arg **ap){
    size_t l = 0;
    for(const char *c = fmt; *c != '\0'; c++){
        if(*c == '%'){
            const pu_arg *a = (*ap)++;
            l += a->typed_snprint(b + l, max > l ? max - l : 0, a->val);
        }
        else if(l++ < max)
            b[l - 1] = *c;
    }
    if(l < max)
        b[l] = '\0';
    return l;
}


size_t _pusnprintf(char *b, size_t max, const char *fmt, const pu_arg *a){
    return  __pusnprintf(b, max, fmt, &a);
}

#define PU_DFLT_BUF_SZ 80
size_t _puprintf(const char *fmt, const pu_arg *a){
    size_t need;
    size_t max = PU_DFLT_BUF_SZ;
    for(int i = 0; i < 2; i++){
        char b[max];
        need = 1 + __pusnprintf(b, max, fmt, &a);
        if(max >= need){
            fputs(b, stdout);
            break;
        }
        max = need;
    }
    return need;
}

#endif
