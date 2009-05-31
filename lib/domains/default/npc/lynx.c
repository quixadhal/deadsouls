#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("lynx");
    SetId(({"npc","mob","quadruped","mobile"}));
    SetAdjectives(({"non-player", "endothermic","carnivorous"}));
    SetShort("a lynx");
    SetLong("An endothermic quadruped, carnivorous by nature, it's roughly as big as a medium-sized dog. It has huge ears it uses to hunt in the dark with.");
    SetWanderSpeed(10);
    SetLevel(1);
    SetMelee(1);
    SetRace("cat");
    SetGender("male");
}
void init(){
    ::init();
}
