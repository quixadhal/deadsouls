#include <lib.h>

inherit LIB_CIGAR;

static void create() {
    ::create();
    SetKeyName("cigar");
    SetId( ({ "cigar", "cylinder" }) );
    SetAdjectives( ({ "bitterleaf" }) );
    SetShort("a cigar");
    SetLong("A cylinder of tightly wrapped bitterleaf designed for "
            "combustion and inhalation its smoke.");
    SetRadiantLight(2);
    SetFuelRequired(1);
    SetMaxFuel(300);
    SetFuelAmount(30);
    SetRefuelable(0);
    SetMass(5);
    SetBaseCost("silver",30);
    SetBurntValue(0);
    SetClass(10);
}
void init(){
    ::init();
}
mixed eventBurnOut(){
    eventDarken();
    set_heart_beat(0);
    eventDestruct();
    return 1;
}
