#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("foochy");
    SetAdjectives( ({"overpampered", "ridiculous", "little", "toy", "toylike"}) );
    SetId(({"npc","mob","character","mobile","poodle"}));
    SetShort("Foochy the pink poodle");
    SetLong("A rather ridiculous, obviously overpampered little toylike dog.");
    SetLevel(1);
    SetMelee(1);
    SetRace("dog");
    SetGender("female");
}
void init(){
    ::init();
}
