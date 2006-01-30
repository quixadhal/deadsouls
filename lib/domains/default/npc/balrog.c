#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("balrog");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a fierce balrog");
    SetLong("This is the terrifying demon of your worst Nightmare!");
    SetWanderSpeed(5);
    SetClass("fighter");
    SetMelee(1);
    SetLevel(50);
    SetRace("balrog");
    SetGender("male");
}
void init(){
    ::init();
}
