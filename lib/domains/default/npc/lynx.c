#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("lynx");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a lynx");
    SetLong("This feline is much larger than a cat, but much smaller than a lion...it's roughly as big as a medium-sized dog. It has huge ears it uses to hunt in the dark with.");
    SetWanderSpeed(10);
    SetLevel(1);
    SetMelee(1);
    SetRace("cat");
    SetGender("male");
}
void init(){
    ::init();
}
