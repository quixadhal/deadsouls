#include <lib.h>
#include <save.h>
#include <daemons.h>
#include <privs.h>

inherit LIB_DAEMON;
static string SaveFile;
mapping StatDeviation = ([]);

static void create() {
    daemon::create();
    SaveFile = save_file(SAVE_DEVIATION);
    SetSaveFile(SaveFile);
    if(!file_exists(SaveFile) && file_exists(old_savename(SaveFile))){
        cp(old_savename(SaveFile), SaveFile);
    }
    RestoreObject(SaveFile);
    if(sizeof(StatDeviation)){
        SaveObject(SaveFile);
    }
    call_out("SetDeviations", 10);
}

float GetStatDeviation(int amount){
    float ret;
    switch(amount){
        case 0 : ret = 0.0; break;
        case 1 : ret = 0.22; break;
        case 2 : ret = 0.44; break;
        case 3 : ret = 0.66; break;
        case 4 : ret = 0.88; break;
        default : ret = 0.99; break;
    }
    return ret;
}

varargs int GetDeviationCost(object who, int xp){
    int dev;
    float fxp, subt;
    string race;
    if(!who || !xp) return 0;
    race = who->GetRace();
    fxp = to_float(xp);
    foreach(mixed key, mixed val in who->GetStatsMap()){
        if(!StatDeviation || !StatDeviation[key] ||
                StatDeviation[key][race]) continue;
        dev = (StatDeviation[key][race] - val["class"]);
        if(dev > 0){
            subt += (fxp * GetStatDeviation(dev));
        }
    }
    return to_int(subt);
}

void SetDeviations(){
    mapping tmpmap = STATS_D->GetStats();
    if(sizeof(tmpmap)) StatDeviation = tmpmap;
    SaveObject(SaveFile);
} 

