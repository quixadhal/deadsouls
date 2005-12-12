#include <lib.h>

inherit LIB_SENTIENT;
static void create() {
    sentient::create();
    SetKeyName("npc");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"generic", "non-player", "non player"}));
    SetShort("a generic npc");
    SetLong("Other than being human, this npc is entirely unremarkable.");
    SetLevel(1);
    SetRace("human");
    SetClass("explorer");
    SetGender("male");
}
