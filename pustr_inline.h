#pragma once
#include <pustr.h>

#define pudispatch_inline(a, num_as, i)                                 \
    case num_as - i - 1:                                                \
           l += pusnprint_of(a)(_b + l,                                 \
                                _max >= l ? _max - l : 0,               \
                                (void *)(typeof(strip_bitfield(a)) []){a}); \
           break;                                                       \

#define pusnprintf_inline(b, max, fmt, as...)                           \
    ({                                                                  \
        unsigned int idx = 0;                                           \
        char *_b = b;                                                   \
        size_t _max = max;                                              \
        const char *_fmt = fmt;                                         \
        size_t l = 0;                                                   \
        for(unsigned int i = 0; i < sizeof(fmt); i++){                  \
            char c = _fmt[i];                                           \
            if(c == '%'){                                               \
                switch(idx++){                                          \
                    PUMAP_NOCOMMA(pudispatch_inline, PU_NUM_ARGS(as), as) \
                }                                                       \
            }                                                           \
            else if(l++ < _max)                                         \
                (_b)[l - 1] = c;                                        \
        }                                                               \
        l - 1;                                                          \
    })
