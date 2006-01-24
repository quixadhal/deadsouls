#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("ogre");
    SetAdjectives( ({"foul", "stinking", "unlovable", "potbellied", "snaggletoothed", "monstrous"}) );
    SetId( ({"oaf", "beast"}) );
    SetShort("a monstrous ogre");
    SetLong("This is not a lovable oaf. This foul, stinking beast is twice as tall as a human, potbellied and snaggletoothed. Its skin is crawling with vermin and is encrusted with dried body fluids.");
    SetLevel(1);
    SetMelee(1);
    SetRace("ogre");
    SetGender("male");
}
void init(){
    ::init();
}
