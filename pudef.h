#ifndef pudef
#error "pudef ain't defined, yall."
#endif

#ifndef NTYPES
#error "pustr.h ain't included, yall."
#endif

#ifndef NPUSTR

/* Want to compute NTYPES + 1 without incrementing NTYPES, as we want to
   use pusnprintf (which uses NTYPES) in the old scope while defining the
   new. */
#if !NEXTNTYPES
#define NEXTNTYPES 1
#elif NEXTNTYPES == 1
#undef NEXTNTYPES
#define NEXTNTYPES 2
#elif NEXTNTYPES == 2
#undef NEXTNTYPES
#define NEXTNTYPES 3
#elif NEXTNTYPES == 3
#undef NEXTNTYPES
#define NEXTNTYPES 4
#elif NEXTNTYPES == 4
#undef NEXTNTYPES
#define NEXTNTYPES 5
#elif NEXTNTYPES == 5
#undef NEXTNTYPES
#define NEXTNTYPES 6
#elif NEXTNTYPES == 6
#undef NEXTNTYPES
#define NEXTNTYPES 7
#elif NEXTNTYPES == 7
#undef NEXTNTYPES
#define NEXTNTYPES 8
#elif NEXTNTYPES == 8
#undef NEXTNTYPES
#define NEXTNTYPES 9
#elif NEXTNTYPES == 9
#undef NEXTNTYPES
#define NEXTNTYPES 10
#elif NEXTNTYPES == 10
#undef NEXTNTYPES
#define NEXTNTYPES 11
#elif NEXTNTYPES == 11
#undef NEXTNTYPES
#define NEXTNTYPES 12
#elif NEXTNTYPES == 12
#undef NEXTNTYPES
#define NEXTNTYPES 13
#elif NEXTNTYPES == 13
#undef NEXTNTYPES
#define NEXTNTYPES 14
#elif NEXTNTYPES == 14
#undef NEXTNTYPES
#define NEXTNTYPES 15
#elif NEXTNTYPES == 15
#undef NEXTNTYPES
#define NEXTNTYPES 16
#elif NEXTNTYPES == 16
#undef NEXTNTYPES
#define NEXTNTYPES 17
#elif NEXTNTYPES == 17
#undef NEXTNTYPES
#define NEXTNTYPES 18
#elif NEXTNTYPES == 18
#undef NEXTNTYPES
#define NEXTNTYPES 19
#elif NEXTNTYPES == 19
#undef NEXTNTYPES
#define NEXTNTYPES 20
#elif NEXTNTYPES == 20
#undef NEXTNTYPES
#define NEXTNTYPES 21
#elif NEXTNTYPES == 21
#undef NEXTNTYPES
#define NEXTNTYPES 22
#elif NEXTNTYPES == 22
#undef NEXTNTYPES
#define NEXTNTYPES 23
#elif NEXTNTYPES == 23
#undef NEXTNTYPES
#define NEXTNTYPES 24
#elif NEXTNTYPES == 24
#undef NEXTNTYPES
#define NEXTNTYPES 25
#else
#error "So many types! Too many to count!"
#endif  /* NEXTNTYPES */

#define PU_FIRST(a, ...) a
#define PU_SECOND(_, a, ...) a
#define PU_TAIL(_, __, a...) a

/* pudef needs to be passed as an arg for it to be expanded? */
#define PU_T(d) PU_FIRST d
#define PU_FMT(d) PU_SECOND d
#define PU_FARGS(d) PU_TAIL d

typedef PU_T(pudef) CONCAT(putype_, NEXTNTYPES);

static inline size_t CONCAT(pusnprint_, PU_T(pudef))
(char *b, size_t l, volatile const PU_T(pudef) *a){                 
    return pusnprintf(b, l, PU_FMT(pudef), PU_FARGS(pudef));
}

static inline size_t CONCAT(pusnprint_ptr_, PU_T(pudef))
(char *b, size_t l, volatile const PU_T(pudef) **apt){
    volatile const PU_T(pudef) *a = *apt;
    if(!a)
        return (size_t) snprintf(b, l, "("STRLIT(PU_T(pudef))" *)<nil>");
    return pusnprintf(b, l, "%:&"PU_FMT(pudef),
                      (volatile const void *) *apt, PU_FARGS(pudef));
}

static inline size_t CONCAT(pusnprint_, NEXTNTYPES)
(char *b, size_t l, volatile const PU_T(pudef) *a){                 
    return CONCAT(pusnprint_, PU_T(pudef))(b, l, a);
}

static inline size_t CONCAT(pusnprint_ptr_, NEXTNTYPES)             
(char *b, size_t l, volatile const PU_T(pudef) **apt){
    return CONCAT(pusnprint_ptr_, PU_T(pudef))(b, l, apt);
}

#if !NTYPES
#undef NTYPES
#define NTYPES 1
#elif NTYPES == 1
#undef NTYPES
#define NTYPES 2
#elif NTYPES == 2
#undef NTYPES
#define NTYPES 3
#elif NTYPES == 3
#undef NTYPES
#define NTYPES 4
#elif NTYPES == 4
#undef NTYPES
#define NTYPES 5
#elif NTYPES == 5
#undef NTYPES
#define NTYPES 6
#elif NTYPES == 6
#undef NTYPES
#define NTYPES 7
#elif NTYPES == 7
#undef NTYPES
#define NTYPES 8
#elif NTYPES == 8
#undef NTYPES
#define NTYPES 9
#elif NTYPES == 9
#undef NTYPES
#define NTYPES 10
#elif NTYPES == 10
#undef NTYPES
#define NTYPES 11
#elif NTYPES == 11
#undef NTYPES
#define NTYPES 12
#elif NTYPES == 12
#undef NTYPES
#define NTYPES 13
#elif NTYPES == 13
#undef NTYPES
#define NTYPES 14
#elif NTYPES == 14
#undef NTYPES
#define NTYPES 15
#elif NTYPES == 15
#undef NTYPES
#define NTYPES 16
#elif NTYPES == 16
#undef NTYPES
#define NTYPES 17
#elif NTYPES == 17
#undef NTYPES
#define NTYPES 18
#elif NTYPES == 18
#undef NTYPES
#define NTYPES 19
#elif NTYPES == 19
#undef NTYPES
#define NTYPES 20
#elif NTYPES == 20
#undef NTYPES
#define NTYPES 21
#elif NTYPES == 21
#undef NTYPES
#define NTYPES 22
#elif NTYPES == 22
#undef NTYPES
#define NTYPES 23
#elif NTYPES == 23
#undef NTYPES
#define NTYPES 24
#elif NTYPES == 24
#undef NTYPES
#define NTYPES 25
#endif  /* NTYPES */

#endif  /* NPUSTR */

#undef pudef

