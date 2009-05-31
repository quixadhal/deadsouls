#include <lib.h>
#include <save.h>
#include <daemons.h>
#include <commands.h>

inherit LIB_DAEMON;

int t, BootScore, PerformanceScore, hasrun = 0;

string RateScore(int i){
    string ret;
    if(!i) ret = "No score available.";
    else if(i < 51) ret = "Spectacular.";
    else if(i < 101) ret = "Excellent.";
    else if(i < 201) ret = "Very good.";
    else if(i < 301) ret = "Good.";
    else if(i < 401) ret = "Fair.";
    else if(i < 501) ret = "Poor.";
    else if(i < 601) ret = "Very poor.";
    else if(i < 701) ret = "Bad.";
    else ret = "Execrable.";
    return ret;
}

static void eventRun() {
    string ret = "";
    mapping before, after;
    string *noobnames = ({ "Dead_Souls_"+DEBUGGER, "DeadSoulsNew",
            "DeadSoulsWin" });
    debug_message("\nRunning autoexec, please wait...");
    BootScore = MASTER_D->GetBootScore();
#ifdef __HAS_RUSAGE__
    before = rusage();
#endif
    t = time(); 
    load_object(CMD_UPDATE)->cmd("-r /lib/creator");
    if(RESET_INTERMUD){
        rm(save_file(SAVE_INTERMUD));
        update(INTERMUD_D);
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
    call_out("perfreport", 1);
}

void perfreport(){
    string ret = "";
    float secs;
    if(!PerformanceScore) PerformanceScore = MASTER_D->GetPerformanceScore();
    if(!BootScore) BootScore = MASTER_D->GetBootScore();
    if(BootScore){
        secs = ((BootScore + t ) * 0.001);
        ret += "CPU time in boot: " + sprintf("%.2f",secs)+ " seconds.";
    }
    if(PerformanceScore){
        ret += " (Performance score: " + (RateScore(PerformanceScore))+")\n";
    }
    debug_message(ret);
}

static void create() {
    daemon::create();
    PerformanceScore = MASTER_D->GetPerformanceScore();
    if(hasrun){
        return;
    }
    call_out((: eventRun :), 1);
    hasrun = 1;
}

