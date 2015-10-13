#pragma once

#ifndef __ASSEMBLER__

#define ainline __attribute__((always_inline))
#define noinline __attribute__((noinline))
#define pure __attribute__((pure))
#define constfun __attribute__((const))
#define noreturn __attribute__((noreturn))
#define packed __attribute__((packed))
#define flat __attribute__((flatten))
#define align(ment) __attribute__((__aligned__(ment)))
#define aliasing __attribute__((may_alias))
#define checked __attribute__((warn_unused_result))
#define dbg __attribute__((unused))
#define transparent __attribute__((transparent_union))

#include <whtypes.h>
#include <limits.h>

#include <string.h>
#include <stdio.h>
#include <pustr.h>

#include <assert.h>
#include <config.h>
#include <log.h>

#include <errors.h>
#include <dial_util.h>
#include <wrand.h>

#include <syscall.h>

#include <atomics.h>

#include <runtime_config.h>    
#include <stdlib.h>

#endif
