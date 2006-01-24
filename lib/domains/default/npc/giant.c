#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("giant");
    SetAdjectives( ({"jolly", "green"}) );
    SetId(({"npc","mob","character","mobile"}));
    SetShort("a green giant");
    SetLong("This massive being is well over fifteen feet tall. He doesn't seem hostile though. Just rather amused, in fact. His skin is entirely green and his clothes are made from the leaves of what must be some gigantic palm trees.");
    SetLevel(1);
    SetMelee(1);
    SetRace("giant");
    SetGender("male");
}
void init(){
    ::init();
}
