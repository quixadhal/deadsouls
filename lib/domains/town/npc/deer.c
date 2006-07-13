#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("deer");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a deer");
    SetLong("A Typical creature of its kind: nervous, shify, yet beautiful and graceful. This gentle herbivore just wants to eat plants and reproduce.");
    SetWimpy(95);
    SetPacifist(1);
    SetWanderSpeed(4);
    SetLevel(1);
    SetMelee(1);
    SetRace("deer");
    SetGender("female");
}
void init(){
    ::init();
}
