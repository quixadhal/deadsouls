#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("newt");
    SetAdjectives( ({"slimy"}) );
    SetId(({"npc","mob","character","mobile"}));
    SetShort("a newt");
    SetLong("A slimy amphibian.");
    SetWanderSpeed(3);
    SetMelee(1);
    SetLevel(1);
    SetRace("amphibian");
    SetClass("explorer");
    SetGender("male");
}
void init(){
    ::init();
}
