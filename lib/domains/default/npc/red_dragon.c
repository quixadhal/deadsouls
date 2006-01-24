#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("dragon");
    SetAdjectives( ({"huge", "red", "dangerous"}) );
    SetId(({"npc","mob","character","mobile"}));
    SetShort("a red dragon");
    SetLong("Ancient villain of legend and fable, the red dragon is among the most vicious, cruel, and dangerous beasts around. This one is easily fifteen feet tall and could probably crush you without even noticing.");
    SetClass("fighter");
    SetLevel(15);
    SetMelee(1);
    SetRace("dragon");
    SetGender("male");
}
void init(){
    ::init();
}
