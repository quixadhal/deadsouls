#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("elemental");
    SetId( ({"transcendental", "manifestation"}) );
    SetAdjectives( ({"boredom"}) );
    SetShort("a boredom elemental");
    SetLong("Normally elementals are summoned creatures composed of one of the four elements: earth, water, wind, or fire. This bizarre manifestation appears to be none of those, and in fact seems to have coalesced unbidden out of a sheer, transcendent boredom. It is, perhaps, more of a boredom transcendental.");
    SetLevel(1);
    SetMelee(1);
    SetRace("elemental");
    SetGender("neuter");
}
void init(){
    ::init();
}
