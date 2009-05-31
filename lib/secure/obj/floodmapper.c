#include <lib.h>
#include <daemons.h>
#include ROOMS_H

inherit LIB_FLOW;

void create() {
    ::create();
    set_heart_beat(1);
    SetDoorStopped(0);
    enable_commands();
    set_heart_beat(1);
    SetHBOverride(1);
}

void init(){
    ::init();
}

void heart_beat(){
    ::heart_beat();
    set_heart_beat(1);
}

int GetMapper(){
    return 1;
}

int GetPressure(){
    return 5;
}

int eventMove(mixed dest){
    int ret;
    string *avoids = ({ "/domains/town/virtual/space", 
            "/domains/town/virtual/surface",
            "/domains/town/virtual/sub", "/domains/town/virtual/bottom" });
    object envr = environment();
    string location, sdest;

    if(objectp(dest)) sdest = base_name(dest);
    else sdest = dest;
    foreach(mixed avoid in avoids){
        if(!strsrch(sdest, avoid)){
            this_object()->eventDestruct();
            return 0;
        }
    }

    if(envr){
        if(clonep(envr)) location = file_name(envr);
        else location = base_name(envr);
    }

    if(location) this_object()->SetProperty("LastLocation", location);
    ret = ::eventMove(dest);
    return ret;
}

