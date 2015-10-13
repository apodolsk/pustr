#pragma once

#include <tid.h>

void breakpoint(void);

bool interrupts_enabled(void);

struct cpu;
struct thread;

struct thread *this_thread(void);
void slabs_init(void);
struct slab *new_slabs(cnt batch);

dbg_id get_dbg_id(void);
void set_dbg_id(dbg_id id);

err pause_universe(void);
void resume_universe(void);

void *heap_start(void);
void *heap_end(void);

bool poisoned(void);
