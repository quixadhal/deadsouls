#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("snake");
    SetId( ({"garter snake", "reptile"}) );
    SetAdjectives( ({"small", "harmless", "garter", "black", "black and yellow", "yellow and black"}) );
    SetShort("a garter snake");
    SetLong("This is a small, harmless reptile, about 9 inches long. It is black with a few yellow stripes along its length. This snake enjoys eating garden pests.");
    SetLevel(1);
    SetMelee(1);
    SetRace("human");
    SetGender("male");
}
void init(){
    ::init();
}
