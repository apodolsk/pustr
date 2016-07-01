#include "pustr.h"

#ifdef NPUSTR
#define putrace(_, f, as...) f(as)
#else

/* C11 doesn't allow 'void' to occur in the list of _Generic types, but
   clang and gcc allow it in __builtin_types_compatible_p. I use this to
   allow putrace to report and propogate the value of fun(as) iff
   typeof(fun(as)) != void. */
#define choose __builtin_choose_expr
#define compat __builtin_types_compatible_p
#define is_void(e) compat(void, typeof(e))

#define pu_strfmt(_, __, ___) %

/* To avoid double-eval of function arg expressions. */
#define estore(e, pfx, i) __auto_type PU_CONCAT(pfx, i) = e;
#define eref(_, pfx, i) PU_CONCAT(pfx, i)

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
