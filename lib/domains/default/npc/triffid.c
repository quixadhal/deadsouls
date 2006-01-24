#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("triffid");
    SetAdjectives( ({"six foot tall", "six feet tall", "thick", "bushy", "thorned", "menacing"}) );
    SetId( ({"plant"}) );
    SetShort("a large, tall plant");
    SetLong("This plant is about six feet tall and similar to a very thick and bushy corn plant. It possesses tentacle-like thorned vines which wave about menacingly, and it appears capable of locomotion.");
    SetLevel(1);
    SetMelee(1);
    SetRace("plant");
    SetGender("neuter");
}
void init(){
    ::init();
}
