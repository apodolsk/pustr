#include "pustr.h"

#ifdef NPUSTR
#define putrace(_, f, as...) f(as)
#else

#if TRACE_START
#define putrace(print, fun, as...)                                      \
    ({                                                                  \
        PUMAP_NOCOMMA(estore, __pu, as);                                \
        print("-- Begin %(" STRLIT(PUMAP(pu_strfmt, _, as))             \
              ") in %:%", #fun                                          \
              COMMAPFX_IF_NZ(PUMAP3(eref, __pu, as)),                   \
              __func__, __LINE__);                                      \
        typeof(choose(is_void(fun(as)),                                 \
                      1,                                                \
                      fun(as)))                                         \
            __pu_ret;                                                   \
        __pu_ret =                                                      \
        choose(is_void(fun(as)),                                        \
               (fun(PUMAP(eref, __pu, as)), 0),                         \
               fun(PUMAP(eref, __pu, as)));                             \
        choose(is_void(fun(as)),                                        \
               ({                                                       \
                   print("-- End %() in %:%", #fun,                     \
                         __func__, __LINE__);                           \
               }),                                                      \
               ({                                                       \
                   print("-- End % = %("STRLIT(PUMAP(pu_strfmt, _, as)) \
                         ") in %:%", __pu_ret, #fun                     \
                         COMMAPFX_IF_NZ(PUMAP3(eref, __pu, as)),        \
                         __func__, __LINE__);                           \
                   __pu_ret;                                            \
               }));                                                     \
    })
#else
#define putrace(print, fun, as...)                                      \
    ({                                                                  \
        PUMAP_NOCOMMA(estore, __pu, as);                                \
        typeof(choose(is_void(fun(as)),                                 \
                      1,                                                \
                      fun(as)))                                         \
        __pu_ret =                                                      \
        choose(is_void(fun(as)),                                        \
               (fun(PUMAP(eref, __pu, as)), 0),                         \
               fun(PUMAP(eref, __pu, as)));                             \
        choose(is_void(fun(as)),                                        \
               ({                                                       \
                   print("%("STRLIT(PUMAP(pu_strfmt, _, as))            \
                         ") in %:%:%", #fun                             \
                         COMMAPFX_IF_NZ(PUMAP3(eref, __pu, as)),        \
                         __FILE__, __func__, __LINE__);                 \
               }),                                                      \
               ({                                                       \
                   print("% = %("STRLIT(PUMAP(pu_strfmt, _, as))        \
                         ") in %:%:%", __pu_ret, #fun                   \
                         COMMAPFX_IF_NZ(PUMAP3(eref, __pu, as)),        \
                         __FILE__, __func__, __LINE__);                 \
                   __pu_ret;                                            \
               }));                                                     \
    })
#endif

#endif  /* !NPUSTR */
