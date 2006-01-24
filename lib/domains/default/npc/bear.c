#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("bear");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a bear");
    SetLong("A large brown bear. Not as huge as a grizzly, but plenty big enough to knock your block off.");
    SetWanderSpeed(20);
    SetClass("fighter");
    SetLevel(10);
    SetMelee(1);
    SetRace("bear");
    SetGender("male");
}
void init(){
    ::init();
}
