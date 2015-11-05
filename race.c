#include <time.h>
#include <wrand.h>
#include <tid.h>
#include <thread.h>

extern err kyield(tid t);
void race(cnt maxns, uint pcnt, uint idmod){
    if(!pcnt)
        return;
    bool r = T->racing;
    T->racing = true;
    if(!r && randpcnt(pcnt)){
        kyield((dptr) -1);
        T->racing = false;
    }
    /* if(!T->racing && interrupts_enabled() && randpcnt(pcnt >> 1)){ */
    /*     (kyield)((dptr) -1); */
    /* }else if(!mod_pow2(PUN(uptr, get_dbg_id()), idmod) && randpcnt(pcnt)) */
    /*     nanosleep(&(struct timespec){.tv_nsec = rand() % maxns}, NULL); */
}
