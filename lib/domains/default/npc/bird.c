#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("dodo");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("a fat little dodo bird");
    SetLong("This foolish little creature's only crime was being born trusting and delicious.");
    SetWanderSpeed(10);
    SetLevel(1);
    SetMelee(1);
    SetRace("bird");
    SetMaxHealthPoints(30);
    SetGender("male");
}
void init(){
    ::init();
}
