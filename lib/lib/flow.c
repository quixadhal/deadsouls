/* Eventually this will handle other kinds of flow
 * materials, but for now it is hardcoded just to be water.
 * Handle this with EXTREME care. It can very easily
 * overwhelm your mud.
 */

#include <lib.h>
#include ROOMS_H
#include <vendor_types.h>
#include <meal_types.h>
#include <medium.h>
#include <respiration_types.h>

inherit LIB_FLASK;
mixed *neighbors = ({});
mixed *rooms = ({});
mixed *first = ({});
mixed *second = ({});
mixed *third = ({});
mixed *fourth = ({});
static int press;
static string FlowType = "water";
static string ThisFile;
static int HBOverride, orig_medium, orig_resp, DoorStopped = 1;

void shb(int i){
    if(!HBOverride) set_heart_beat(i);
}

void create(){
    ::create();
    SetKeyName("water");
    SetId( ({"flow","flood","object","water","overflow"}) );
    SetAdjectives( ({"flow","flood"}) );
    SetShort("some water");
    SetLong("This is an overflow of water.");
    SetMass(20);
    SetInvis(1);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    shb(5);
    SetNoCondition(1);
    SetPreventGet("You can't get that.");
    if(FlowType == "water"){
        SetFlaskContents("water");
        SetFlaskUses(2);
        SetStrength(5);
        SetMaxFlask(2);
        SetEverFill(1);
        SetMealType(MEAL_DRINK);
    }
    ThisFile = base_name(this_object());
}

void eventFlood(mixed targets){
    object env = environment();
    if(!env) return;
    foreach(mixed room in scramble_array(targets)){
        mixed flooder;
        object arch = load_object(ROOM_ARCH);
        if(!room) break;
        if(environment() == room) continue;
        if(press < 2) break;
        flooder = filter(all_inventory(room),
                (: base_name($1) == ThisFile :) );
        if(sizeof(flooder)){
            if(sizeof(flooder) > 1) flooder[1..]->eventDestruct();
            flooder = flooder[0];
        }
        else {
            int flood;
            flooder = new(ThisFile);
            flooder->SetProperty("LastLocation", base_name(env)); 
            flood = flooder->eventMove(room);
            if(!flood) continue;
            tell_room(room,"This area starts flooding with water!");
            tell_room(arch,base_name(room)+" has been flooded.");
        }
        if(flooder->GetPressure() < press){
            if(press && flooder->AddPressure(1)) press--;
            if(press < 2){ 
                tell_room(env,"The water recedes a bit.");
            }
        }
    }
}

void CheckRooms(){
    mixed ret = ({});
    object env;
    mapping Exits;
    env = room_environment();
    if(press < 1){
        eventDestruct();
        return;
    }
    if(clonep() && environment() && !living(environment()) && press > 1){
    }
    if(env){
        Exits = env->GetExitMap(); 
        Exits += env->GetEnterMap(); 
        foreach(mixed key, mixed val in Exits){
            mixed door = env->GetDoor(key);
            int ok =1;
            if(door){
                if(DoorStopped && door->GetClosed() && !door->GetPerforated()){
                    ok = 0;
                }
                else{
                }
            }
            if(ok) ret += ({ val });
        }
    }
    if(sizeof(ret)) rooms = ret;
    if(press > 1){
        rooms = ({});
        foreach(mixed key, mixed val in Exits){
            if(member_array(val, ret) == -1) continue;
            if(key == "down") first += ({ load_object(val) });
            else if(key == "up") fourth += ({ load_object(val) });
            else rooms += ({ load_object(val) });
        }
        foreach(mixed room in rooms){
            mixed flooder;
            if(press < 2) break;
            flooder = filter(all_inventory(room),
                    (: base_name($1) == ThisFile :) );
            if(!sizeof(flooder)) second += ({ room });
            else third += ({ room });
        }
        if(sizeof(first) && press > 0) eventFlood(first);
        if(sizeof(second) && press > 1) eventFlood(second);
        if(sizeof(third) && press > 1) eventFlood(third);
        if(sizeof(fourth) && press > 2) eventFlood(fourth);
    }
}

void heart_beat(){
    object env;
    if(!clonep()) return;
    env = environment();
    if(!env || living(env) || press < 2){
        shb(240);
        return;
    }
    CheckRooms();
    if(press > 1 && env){
        env->SetRespirationType(R_WATER);
    }
    else if(env){
        env->SetRespirationType(orig_resp);
    }
    shb(90);
}

int AddPressure(int x){
    int i;
    object env = environment();
    if(!env) return 0;
    i = env->GetFlowLimit();
    if(i && x > 0 && press >= i) return 0;
    if(x){
        if(press == 1 && x > 0){
            tell_room(env,"The area completely fills with water!");
        } 
        if(press > 1 && x < 0){
            tell_room(env,"The water recedes a bit.");
        }
        press += x;
    }
    return press;
}

varargs int eventPrint(string msg, mixed arg2, mixed arg3){
    msg = lower_case(msg);
    if(grepp(msg, "opens") || grepp(msg, "closes") ){
        CheckRooms();
        shb(5);
    }
    return 1;
}

int GetPressure(){
    return press;
}

int SetPressure(int i){
    press = i;
    return press;
}

int GetHBOverride(){
    return HBOverride;
}

int SetHBOverride(int x){
    if(x) HBOverride = 1;
    else HBOverride = 0;
    return HBOverride;
}

int GetDoorStopped(){
    return DoorStopped;
}

int SetDoorStopped(int i){
    DoorStopped = i;
    return DoorStopped;
}

void init(){
    ::init();
}

string GetRoomAffectLong(){
    if(press > 1){
        return "\n%^CYAN%^This area is completely filled with water!%^RESET%^";
    }
    if(press == 1){
        return "\n%^BOLD%^%^BLUE%^This area is flooded with water.%^RESET%^";
    }
    return "";
}

int eventDestruct(){
    object env = environment();
    if(env){
        env->SetRespirationType(orig_resp);
    }
    return ::eventDestruct();
}

int eventMove(mixed dest){
    object env;
    int ret = ::eventMove(dest);
    if(ret && env = room_environment(this_object())){
        orig_resp = env->GetRespirationType();
    }
    return ret;
}

int GeteventPrints() { return 1; }

