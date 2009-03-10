#include <lib.h>
#include <save.h>
#include <daemons.h>

inherit LIB_DAEMON;

int hasrun = 0;

string RateScore(int i){
    string ret;
    if(!i) ret = "No score available.";
    else if(i < 500) ret = "Spectacular.";
    else if(i < 1000) ret = "Excellent.";
    else if(i < 1500) ret = "Good.";
    else if(i < 3000) ret = "Fair.";
    else if(i < 5000) ret = "Poor.";
    else ret = "Very poor.";
    return ret;
}

static void eventRun() {
    string ret = "";
    int t, PerformanceScore = MASTER_D->GetPerformanceScore();
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
        rm(save_file(SAVE_INTERMUD));
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
    if(PerformanceScore){
        float secs = ((PerformanceScore + t ) * 0.001);
        ret += "CPU time in boot: " + sprintf("%.2f",secs)+ " seconds.";
        ret += " (Performance score: " + (RateScore(PerformanceScore))+")\n";
    }
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

