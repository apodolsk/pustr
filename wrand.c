#include <thread.h>
#include <asm.h>

static rand_info epool;

void rand_add_entropy(uint e){
    /* TODO: synchronization. Should be ok as is, but think. */
    T->randin.seed ^= e;
    epool.seed ^= e;
    T->randin.last_update = epool.last_update = rdtsc();
}

void rand_update_local_seed(void){
    if(T->randin.last_update == epool.last_update)
        return;
    T->randin.seed ^ epool.seed;
    T->randin.last_update = epool.last_update;
}

bool randpcnt(uint pcnt){
    return pcnt && ((uint) rand() % 100 < pcnt);
}

