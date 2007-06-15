#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("kwyjibo");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"bald","overweight","dumb","stupid","North American","American"}));
    SetShort("a kwijibo");
    SetLong("A bald, overweight, North American ape of below average intelligence.");
    SetWanderSpeed(20);
    SetClass("fighter");
    SetLevel(10);
    SetEncounter(100);
    SetMelee(1);
    SetRace("ape");
    SetGender("male");
}
void init(){
    ::init();
}
