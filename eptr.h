#pragma once

/* Expands to the type of a pointer to t or else a pointer to NULL or else
   an err. Compatible with t *.

   It doesn't replace NULL, and non-eptr-pointers can still be NULL. I
   also do allow eptrs to be NULL, but it's probably a mistake.
   
   It's an alternative to errno-based control flow and "return buffer"
   function args. It makes me less anxious.

   You could do unions to be stricter, but it's t * to stay in line with
   the standard "hidden-NULL-option" pointer style. Footshooty, but much
   easier to work with syntactically. */
#define eptr(t) t *

static inline constfun
err eof(const volatile void *ep){
    return (iptr) ep > EMAX && (iptr) ep < EMIN
           ? 0
           : 0 - (iptr) ep;
}
