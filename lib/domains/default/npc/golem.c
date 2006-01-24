#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("golem");
    SetAdjectives( ({"massive", "clay", "tall"}) );
    SetId( ({"clay man", "clayman", "klaymen", "emet"}) );
    SetShort("Emet the golem");
    SetLong("Emet is a massive clay man, some ten feet tall and with great, thick arms and legs. Written on his forehead are some cryptic glyphs.");
    SetLevel(10);
    SetMelee(1);
    SetRace("golem");
    SetGender("neuter");
}
void init(){
    ::init();
}
