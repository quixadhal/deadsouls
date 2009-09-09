#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("generic npc");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non player"}));
    SetShort("a generic npc");
    SetLong("Other than being human, this npc is entirely unremarkable.");
    SetLevel(1);
    SetMelee(1);
    SetRace("human");
    SetGender("male");
}
void init(){
    ::init();
}
