#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("gecko");
    SetAdjectives( ({"gecko", "green", "small"}) );
    SetId( ({"lizard"}) );
    SetShort("a gecko");
    SetLong("This is a small lizard, about four inches in length. It has green skin, and unusually large eyes.");
    SetWimpy(80);
    SetLevel(1);
    SetMelee(1);
    SetRace("lizard");
    SetMaxHealthPoints(10);
    SetGender("male");
}
void init(){
    ::init();
}
