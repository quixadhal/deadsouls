// For examples of valid trap types, see /include/boobytraps.h
//
// Boobytraps are two objects. The "pre" trap object, which is the tangible
// object you can hold. Then there is the "post" object, an invisible
// shadow that attaches itself to the target. This is the "pre" object. Once
// the trap is set, this object is removed from the game, and the shadow
// object attaches itself to the target, waiting for the appropriate stimulus.
//
// The shadow object is indicated in the SetShadowObject directive.

#include <lib.h>
#include <boobytraps.h>

inherit LIB_BOOBYTRAP_OBJECT;

void create(){
    ::create();
    SetKeyName("trap");
    SetId( ({ "trap" }) );
    SetAdjectives( ({ "simple","weapon", "poison", "needle" }) );
    SetShort("a weapon trap");
    SetLong("It is a simple poison needle trap designed to trigger when a "
            "weapon is wielded.");
    SetShadowObject("/shadows/needle_trap");
    SetTrapType(BOOBYTRAP_WIELD);
    SetMass(50);
    SetAutoResets(2);
    SetBaseCost(100);
}

void init(){
    ::init();
}
