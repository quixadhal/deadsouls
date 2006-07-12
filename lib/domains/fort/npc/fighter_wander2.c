#include <lib.h>
#include "../customdefs.h"

inherit LIB_SENTIENT;

static void create(){
    sentient::create();
    SetKeyName("fighter");
    SetId( ({"human","npc","warrior","fighter"}) );
    SetShort("a fighter");
    SetLong("This is a sample warrior SENTIENT.");
    SetLevel(10);
    SetRace("human");
    SetClass("fighter");
    SetGender("male");
    SetWanderSpeed(10);
    SetInventory(([
	MY_ARMOR "/chainmail.c" : "wear chainmail",
	MY_ARMOR "/leather_boot_r.c" : "wear boot",
	MY_ARMOR "/leather_boot_l.c" : "wear boot",
	MY_WEAP "/sword.c" : "wield sword",
      ]));
}
void init(){
    ::init();
}
varargs int eventDie(object agent){
    new(base_name(this_object()))->eventMove(environment(this_object()));
    new(base_name(this_object()))->eventMove(environment(this_object()));
    ::eventDie(agent);
}

