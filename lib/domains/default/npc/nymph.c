#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("claire");
    SetId( ({"nymph", "danes", "claire danes", "woman"}) );
    SetAdjectives(({"non-player", "non player"}));
    SetShort("Claire Danes");
    SetLong("This slender nature spirit appears quite lost, and probably should be accompanying a lesser goddess through woodland glades.");
    SetLevel(1);
    SetMelee(1);
    SetRace("nymph");
    SetGender("female");
}
void init(){
    ::init();
}
