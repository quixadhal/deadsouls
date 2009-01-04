#include <lib.h>
#include <daemons.h>
#include <config.h>

inherit LIB_DAEMON;

int hasrun = 0;

static void eventRun() {
    string ret = "";
    int t;
    mapping before, after;
    string *noobnames = ({ "Dead_Souls_"+DEBUGGER, "DeadSoulsNew",
            "DeadSoulsWin" });
    debug_message("\nRunning autoexec, please wait...");
#ifdef __HAS_RUSAGE__
    before = rusage();
#endif
    t = time(); 
    load_object("/secure/cmds/creators/update")->cmd("-r /lib/creator");
    if(RESET_INTERMUD){
        rm("/save/intermud.o");
        update("/daemon/intermud");
    }
    if(member_array(mud_name(),noobnames) == -1){
        mixed foo;
        foo = IMC2_D->UnSetAutoDisabled(1);
    }

    //Put your start-on-boot stuff below

    catch( load_object("/secure/cmds/creators/dsversion")->cmd());
    catch( reload("/domains/default/room/stargate_lab.c",0,1));
    catch( reload("/domains/town/virtual/space/1,1,1",0,1));
    catch( reload("/domains/town/virtual/bottom/33,100000",0,1));
    catch( reload("/domains/Praxis/square.c",0,1));
    catch( reload("/domains/Ylsrim/room/tower",0,1));
    catch( reload("/domains/campus/room/slab",0,1));

    //Put your start-on-boot stuff above

#ifdef __HAS_RUSAGE__
    after = rusage();
#endif
    t = time() - t;
#ifdef __HAS_RUSAGE__
    if(sizeof(before) && sizeof(after)){
        t = after["utime"] - before["utime"];
        ret = "Autoexec daemon run complete. ("+t+"ms)\n";
    }
#else 
    ret =  "Autoexec daemon run complete.\n";
#endif

    debug_message(ret);
}

static void create() {
    daemon::create();
    if(hasrun){
        return;
    }
    call_out((: eventRun :), 1);
    hasrun = 1;
}

