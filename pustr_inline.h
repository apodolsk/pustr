#pragma once
#include <pustr.h>

#define pusnprintf_inline(b, max, fmt, as...)                           \
    ({                                                                  \
        unsigned int _idx = 0;                                          \
        size_t _l = 0;                                                  \
        const pu_arg astart[] = {PUMAP(pu_arg_of, _, as)};              \
        for(const char *c = (fmt); *c != '\0'; c++){                    \
            if(*c == '%'){                                              \
                const pu_arg *_a = &(astart)[_idx++];                    \
                l += _a->typed_snprint((b) + l, (max) > l ? (max) - l : 0, _a->val); \
            }                                                           \
            else if(l++ < max)                                          \
                (b)[l - 1] = *c;                                        \
        }                                                               \
        if(l < max)                                                     \
            (b)[l] = '\0';                                              \
        l;                                                              \
    })
