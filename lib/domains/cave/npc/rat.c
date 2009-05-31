#include <lib.h>

inherit LIB_SENTIENT;

int RatEncounter(mixed val){
    if(!val) return 0;
    if(!objectp(val)) return 0;
    if(val->GetRace() == "orc" || val->GetRace() == "rodent") return 0;
    else {
        if(!this_object()->GetLeader() && sizeof(filter(val->GetFollowers(),
                        (: base_name($1) == base_name(this_object()) :))) < 5){
            eventForce("follow "+val->GetKeyName());
            SetWanderSpeed(1);
        }
        return 1;
    }
    return 0;
}

static void create() {
    sentient::create();
    SetKeyName("rat");
    SetAdjectives( ({"ratlike","unusual","large","very","dangerous",
                "dangerous looking","rat like"}) );
    SetId( ({"rat","rous","beast"}) );
    SetShort("a rodent of unusual size");
    SetLong("A very large, very dangerous-looking ratlike beast.");
    SetRace("rodent");
    SetGender("male");
    SetClass("fighter");
    SetEncounter( (: RatEncounter :));
    SetLevel(3);
    SetMelee(1);
    SetMessage("come","$N scurries in.");
    SetMessage("leave","$N scurries $D.");
}

void init(){
    object env = environment();
    ::init();
    if(env && base_name(env) == "/domains/cave/room/cave"){
        eventForce("go east");
    }
}

void heart_beat(){
    object env = environment();
    ::heart_beat();
    if(env && base_name(env) == "/domains/cave/room/cave"){
        eventForce("go east");
    }
}

