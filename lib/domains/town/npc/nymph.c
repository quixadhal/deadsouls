#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("naiad");
    SetId( ({"nymph", "ludivine", "woman","spirit"}) );
    SetAdjectives(({"water", "beautiful"}));
    SetShort("a beautiful naiad");
    SetLong("This slender nature spirit appears to be a water nymph, protecting this strange pool.");
    SetClass("fighter");
    SetLevel(8);
    SetMelee(1);
    SetRace("nymph");
    SetGender("female");
    SetGuard("down", "The naiad playfully bars your path.");
}

void init(){
    ::init();
}
