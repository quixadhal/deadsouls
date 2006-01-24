#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("minotaur");
    SetId(({"npc","mob","character","mobile"}));
    SetAdjectives(({"non-player", "non player"}));
    SetShort("The Minotaur");
    SetLong("This is the fearsome monster to whom countless virgins were sacrificed. It is transcendentally horrifying, standing eight feet tall with a body seemingly of iron muscle and the head of a raging bull. It is so fearsome as to inspire a kind of hypnotic awe, and it is easy to see it as a force of nature to be worshipped.");
    SetClass("fighter");
    SetLevel(30);
    SetMelee(1);
    SetRace("demi-god");
    SetGender("male");
}
void init(){
    ::init();
}
