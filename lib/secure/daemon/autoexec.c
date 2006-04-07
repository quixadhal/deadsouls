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
    load_object("/secure/cmds/creators/dsversion")->cmd();
}

static void create() {
    daemon::create();
    if(hasrun) return;
    call_out((: eventRun :), 60);
    hasrun = 1;
}

