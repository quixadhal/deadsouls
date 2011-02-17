#include <lib.h>
#include <domains.h>

inherit LIB_NPC;
int lupus;

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
    if(!strsrch(base_name(environment()), DOMAIN_EX)){
        if(!lupus){
            lupus = 1;
            eventForce("say bad wolf!");
        }
        return 0;
    }
    else eventForce("growl at "+val->GetKeyName());
    return 1;
}

static void create() {
    npc::create();
    SetKeyName("orc");
    SetId(({"orc"}));
    SetAdjectives(({"dirty"}));
    SetShort("a dirty orc");
    SetLong("This orc is typical of its breed: nasty, brutish, and short. It appears "
            "to be a juvenile or adolescent, making it somewhat less "
            "dangerous but more hostile. ");
    SetLevel(1);
    SetRace("orc");
    SetGender("male");
    SetMaxHealthPoints(100);
    SetEncounter(  (: CheckOrc :) );
    SetInventory(([
                "/domains/town/weap/dagger":"wield dagger",
                ]) );
    SetGuard("west", "An orc bars your way!" );
    SetGuard("north", "An orc bars your way!");
}
void init(){
    ::init();
}
void reset(){
}
