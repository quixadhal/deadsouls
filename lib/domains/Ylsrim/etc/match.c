/*    /domains/Praxis/etc/torch.c
 *    from Dead Souls LPMud
 *    created by Descartes of Borg 951023
 */

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
    SetMass(5);
    SetValue(2);
    SetBurntValue(1);
}
