#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("dryad");
    SetAdjectives( ({"tree hugging", "tree-hugging", "hippie"}) );
    SetId( ({"hippie"}) );
    SetShort("a tree-hugging hippie");
    SetLong("On closer inspection, this is no mere hippie chick. This is an honest-to goodness tree spirit, evidently a bit lost to be hanging around here.");
    SetLevel(1);
    SetMelee(1);
    SetRace("dryad");
    SetGender("female");
}
void init(){
    ::init();
}
