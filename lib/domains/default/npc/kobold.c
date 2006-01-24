#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("kobold");
    SetAdjectives( ({"lizard", "short", "humanoid"}) );
    SetId( ({"lizard", "lizard man", "creature"}) );
    SetShort("a short lizard man");
    SetLong("This three foot tall lizard creature is a kobold. Its features are like those of a reptile, but it has arms and legs like a humanoid.");
    SetLevel(1);
    SetMelee(1);
    SetRace("kobold");
    SetGender("male");
}
void init(){
    ::init();
}
