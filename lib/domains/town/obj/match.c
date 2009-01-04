#include <lib.h>

inherit LIB_MATCH;

static void create() {
    match::create();
    SetKeyName("match");
    SetId("match");
    SetAdjectives( ({ "wooden" }) );
    SetShort("a wooden match");
    SetLong("A wooden match that might light if you strike it.");
    SetRadiantLight(2);
    SetStrikeChance(50);
    SetMinHeat(10);
    SetFuelRequired(1);
    SetMaxFuel(10);
    SetFuelAmount(10);
    SetRefuelable(0);
    SetMass(1);
    SetBaseCost("silver",1);
    SetBurntValue(1);
}

void init(){
    ::init();
}
