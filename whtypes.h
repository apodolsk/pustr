#pragma once
#ifndef __ASSEMBLER__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef unsigned int uint;
typedef uintptr_t uptr;
/* Counts any set of objects simultaneously in memory. */
typedef uptr cnt;
typedef cnt idx;
typedef intptr_t iptr;
typedef size_t size;
typedef ptrdiff_t ptrdiff;
typedef enum{
    EOK = 0,
    EARG = -1,
    EOOR = -2,
    
    EMAX = EOK,
    EMIN = EOOR,
} err;
typedef int ecnt;

typedef unsigned char u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#if _LP64
typedef __int128_t dptr;
typedef __uint128_t udptr;
#else
typedef int64_t dptr;
typedef uint64_t udptr;
#endif

#define UPTR_MAX UINTPTR_MAX
#define CNT_MAX UPTR_MAX

#define WORDBITS (8 * sizeof(void *))

#endif
