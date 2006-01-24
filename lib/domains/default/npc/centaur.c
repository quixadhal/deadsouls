#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("centaur");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a centaur");
    SetLong("Always a startling sight, this is a creature whose lower part is a stag, but where the head would be is the torso of a man, complete with arms and a head. This deer/man appears quite wild and looks every bit as unpredictable as legend says.");
    SetWanderSpeed(10);
    SetLevel(1);
    SetMelee(1);
    SetRace("centaur");
    SetGender("male");
}
void init(){
    ::init();
}
