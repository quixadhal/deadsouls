#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("cat");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("cat");
    SetLong("Other than being human, this npc is entirely unremarkable.");
    SetLevel(1);
    SetMelee(1);
    SetRace("cat");
    SetGender("female");
}
void init(){
    ::init();
}
