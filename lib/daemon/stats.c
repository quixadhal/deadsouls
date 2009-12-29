#include <lib.h>
#include <save.h>
#include <daemons.h>
#include <privs.h>

inherit LIB_DAEMON;
static string SaveFile;
mapping Stats = ([]);

static void create() {
    daemon::create();
    SaveFile = save_file(SAVE_STATS);
    SetSaveFile(SaveFile);
    if(!file_exists(SaveFile) && file_exists(old_savename(SaveFile))){
        cp(old_savename(SaveFile), SaveFile);
    }
    RestoreObject(SaveFile);
    if(sizeof(Stats)){
        SaveObject(SaveFile);
    }
    else {
        call_out("SetStats", 5);
    }
}

varargs int SetStat(string what, string race, int statcl){
    object prev = previous_object();
    string pbase = base_name(prev);
    int accept_class;
    if(!prev || pbase = RACES_D){
        accept_class = 1;
    } 
    else if(!living(prev)){
        return 0;
    }
    if(!Stats) Stats = ([]);
    if(!Stats[what]) Stats[what] = ([]);
    if(!undefinedp(race) && !undefinedp(statcl)){
        if(!statcl) statcl = 5;
        if(accept_class) Stats[what][race] = statcl;
        return 2;
    }
    return 1;
}

static void SetStats(){
    foreach(string race in RACES_D->GetRaces()){
        mapping stats = RACES_D->GetRace(race)["Stats"];
        foreach(mixed key, mixed val in stats){
            SetStat(key, race, val["Class"]);
        }
    }
}

mapping GetStats(){
    return copy(Stats);
}
