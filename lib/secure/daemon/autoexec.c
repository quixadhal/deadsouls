#include <lib.h>
#include <config.h>

inherit LIB_DAEMON;

int hasrun = 0;

static void eventRun() {
    load_object("/secure/cmds/creators/update")->cmd("-r /lib/creator");
    if(RESET_INTERMUD){
        rm("/save/intermud.o");
        update("/daemon/intermud");
    }
    catch( load_object("/secure/cmds/creators/dsversion")->cmd());
    catch( reload("/domains/default/room/stargate_lab.c"));
    catch( reload("/domains/town/virtual/space/1,1,1"));
    catch( reload("/domains/town/virtual/bottom/33,100000"));
    catch( reload("/domains/Praxis/square.c"));
    catch( reload("/domains/Ylsrim/room/tower"));
    catch( reload("/domains/campus/room/slab"));
}

static void create() {
    daemon::create();
    if(hasrun) return;
    call_out((: eventRun :), 10);
    hasrun = 1;
}

