#include <lib.h>
#include <daemons.h>
#include <config.h>

inherit LIB_DAEMON;

int hasrun = 0;

static void eventRun() {
    string *noobnames = ({ "Dead_Souls_"+DEBUGGER, "DeadSoulsNew",
      "DeadSoulsWin" });
    load_object("/secure/cmds/creators/update")->cmd("-r /lib/creator");
    if(RESET_INTERMUD){
        rm("/save/intermud.o");
        update("/daemon/intermud");
    }
    catch( load_object("/secure/cmds/creators/dsversion")->cmd());
    catch( reload("/domains/default/room/stargate_lab.c",0,1));
    catch( reload("/domains/town/virtual/space/1,1,1",0,1));
    catch( reload("/domains/town/virtual/bottom/33,100000",0,1));
    catch( reload("/domains/Praxis/square.c",0,1));
    catch( reload("/domains/Ylsrim/room/tower",0,1));
    catch( reload("/domains/campus/room/slab",0,1));
    if(member_array(mud_name(),noobnames) == -1){
        mixed foo;
        foo = IMC2_D->UnSetAutoDisabled(1);
    }

    //Put your start-on-boot stuff below:
    debug_message("Autoexec daemon loaded.");

}

static void create() {
    daemon::create();
    if(hasrun){
        return;
    }
    call_out((: eventRun :), 10);
    hasrun = 1;
}

