/*
 * Seeks dummy items without an environment and destroys them.
 * Hackey workaround to sloppy code I'm still tracking down.
 * -Crat 21Jan06
 *
 * Started as a hackey workaround to a dummy problem, but
 * it turns out that periodically destructing  cloned
 * objects without an environment is a pretty good idea.
 * see the reaper.c sefun file for how the dests happen
 * and what objects are exempted.
 * -Crat 28Jul06
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

static void eventReap() {
    call_out((: eventReap :), 300);
    reap_dummies();
    reap_other();
    reclaim_objects();
}

static void CheckMem(){
    call_out((: CheckMem :), 60);
    if(MEMUSE_SOFT_LIMIT && memory_info() > MEMUSE_SOFT_LIMIT){
        reap_dummies();
        reset_eval_cost();
        reap_other();
        reset_eval_cost();
        reclaim_objects();
        reset_eval_cost();
        MASTER_D->RequestReset();
    }
    if(MEMUSE_HARD_LIMIT && memory_info() > MEMUSE_HARD_LIMIT){
        reap_dummies();
        reset_eval_cost();
        reap_other();
        reset_eval_cost();
        reclaim_objects();
        reset_eval_cost();
        MASTER_D->RequestReset();
        if(memory_info() > MEMUSE_HARD_LIMIT){
            if(!EVENTS_D->GetRebooting()){
                EVENTS_D->eventReboot(MINUTES_REBOOT_WARNING);
            }
        }
    }
}

static void create() {
    daemon::create();
    SetNoClean(1);
    call_out((: eventReap :), 300);
    call_out((: CheckMem :), 60);
    set_heart_beat(5);
}

static void heart_beat(){
#ifdef __FLUFFOS__
    if(sizeof(get_garbage()) > 20000){
        reap_other();
    }
#endif
}

int eventDestruct(){
    if( !(master()->valid_apply(({ "SECURE" }))) )
        error("Illegal attempt to destruct reaper: "+get_stack()+" "+identify(previous_object(-1)));
    return ::eventDestruct();
}

