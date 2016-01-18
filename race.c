#include <time.h>
#include <wrand.h>
#include <tid.h>
#include <thread.h>

extern err (kyield)(tid t);
void race(cnt maxns, uint pcnt, uint idmod){
    if(!pcnt || T->racing || !randpcnt(pcnt))
        return;
    T->racing = true;
    if(interrupts_enabled()){
        kyield((dptr) -1);
    }else
        nanosleep(&(struct timespec){.tv_nsec = rand() % maxns}, NULL);
    T->racing = false;
}
