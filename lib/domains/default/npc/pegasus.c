#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("winged horse");
    SetId( ({"horse", "steed", "equine", "pegasus"}) );
    SetAdjectives( ({"flying", "winged", "white"}) );
    SetShort("a winged horse");
    SetLong("This beautiful white steed is a pegasus, a horse endowed with great feathered wings and can fly.");
    SetLevel(5);
    SetMount(1);
    SetRace("pegasus");
    SetStat("strength", 50);
    SetGender("male");
    SetMelee(1);
    SetAction(5, ({ 
                "The winged horse snorts.", "You hear the winged horse flap its wings a bit.", 
                "The winged horse flips its tail in the air.",
                "The winged horse looks around."}));
}
void init(){
    ::init();
}
