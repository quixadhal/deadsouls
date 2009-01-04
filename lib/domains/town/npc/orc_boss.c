#include <lib.h>

inherit LIB_NPC;

int AllowPass(object who, object what){
    string *allowed_races = ({ "orc", "half-orc", "bear" });
    if(member_array(who->GetRace(), allowed_races) != -1) return 1;
    return ::AllowPass(who, what);
}

int CheckOrc(mixed val){
    string *allowed_races = ({ "orc", "half-orc", "bear" });
    if(!val) return 0;
    if(!objectp(val)) return 0;
    if(member_array(val->GetRace(), allowed_races) != -1) return 0;
    else eventForce("growl at "+val->GetKeyName());
    return 1;
}

static void create() {
    npc::create();
    SetKeyName("boss orc");
    SetId(({"orc","boss","boss"}));
    SetAdjectives(({"dirty","orc","boss"}));
    SetShort("the boss orc");
    SetLong("This orc is typical of its breed: nasty, brutish, and short. It is larger "
            "than the local orcs you've seen, and looks mean and tough.");
    SetLevel(1);
    SetRace("orc");
    SetClass("fighter");
    SetGender("male");
    SetMaxHealthPoints(200);
    SetEncounter(  (: CheckOrc :) );
    SetInventory(([
                "/domains/town/weap/dagger":"wield dagger",
                ]) );
    SetGuard("west","The boss orc blocks your path!");
}

void init(){
    ::init();
}
