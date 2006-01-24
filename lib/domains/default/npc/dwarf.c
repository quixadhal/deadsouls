#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("dwarf");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a dwarf");
    SetLong("This is your typical hearty-looking, short, powerfully-built dwarf. Ruddy cheeks, full beard, the works. Straight out of central casting, this one.");
    SetLevel(1);
    SetMelee(1);
    SetRace("dwarf");
    SetGender("male");
}
void init(){
    ::init();
}
