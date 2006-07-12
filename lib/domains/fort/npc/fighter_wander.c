#include <lib.h>
#include "../customdefs.h"

inherit LIB_SENTIENT;

static void create(){
    sentient::create();
    SetKeyName("fighter");
    SetId( ({"human","npc","warrior","fighter"}) );
    SetShort("a fighter");
    SetLong("This is a sample warrior SENTIENT.");
    SetLevel(8);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetWanderSpeed(5);
}
void init(){
    ::init();
}
varargs int eventDie(object agent){
    new(base_name(this_object()))->eventMove(environment(this_object()));
    new(base_name(this_object()))->eventMove(environment(this_object()));
    ::eventDie(agent);
}

