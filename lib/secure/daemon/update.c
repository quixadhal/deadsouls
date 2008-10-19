#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

static void eventUpdate(){}

static void create(){
    object *prevs = previous_object(-1);
    object lu = find_object("/secure/cmds/admins/liveupgrade");
    daemon::create();
    if(lu && sizeof(prevs) && member_array(lu, prevs) != -1){
        cp("/secure/daemon/update.patch","/secure/daemon/update.c");
        RELOAD_D->eventReload("/secure/daemon/update.c", 2);
    }
    else {
        call_out((: eventUpdate :), 60);
    }
}
