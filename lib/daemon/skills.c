#include <lib.h>
#include <save.h>
#include <daemons.h>
#include <privs.h>

inherit LIB_DAEMON;
static string SaveFile;
mapping Skills = ([]);
mapping RacialSkills = ([]);

static void create() {
    daemon::create();
    SaveFile = save_file(SAVE_STATS);
    SetSaveFile(SaveFile);
    if(!file_exists(SaveFile) && file_exists(old_savename(SaveFile))){
        cp(old_savename(SaveFile), SaveFile);
    }
    RestoreObject(SaveFile);
    if(sizeof(Skills)){
        SaveObject(SaveFile);
    }
    else {
        call_out("SetSkills", 5);
    }
}

varargs int SetSkill(string what, string cls, int statcl, int racial){
    object prev = previous_object();
    string pbase = base_name(prev);
    int accept_class;
    if(!prev || pbase = CLASSES_D || pbase = RACES_D){
        accept_class = 1;
    } 
    else if(!living(prev)){
        return 0;
    }
    if(!undefinedp(cls) && !undefinedp(statcl)){
        if(!statcl) statcl = 5;
        if(accept_class){
            if(!racial){
                if(!Skills) Skills = ([]);
                if(!Skills[what]) Skills[what] = ([]);
                Skills[what][cls] = statcl;
            }
            else {
                if(!RacialSkills) RacialSkills = ([]);
                if(!RacialSkills[what]) RacialSkills[what] = ([]);
                RacialSkills[what][cls] = statcl;
            }
        }
        return 2;
    }
    return 1;
}

static void SetSkills(){
    foreach(string cls in CLASSES_D->GetClasses()){
        mapping skills = CLASSES_D->GetClass(cls)["Skills"];
        foreach(mixed key, mixed val in skills){
            SetSkill(key, cls, val["SkillClass"]);
        }
    }
    foreach(string race in RACES_D->GetRaces()){
        mapping skills = RACES_D->GetRace(race)["Skills"];
        foreach(mixed key, mixed val in skills){
            SetSkill(key, race, val[1], 1);
        }
    }
}
