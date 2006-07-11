/*    /domains/Examples/armor/glove.c
 *    from the Dead Souls LPC Library
 *    an example glove
 *    created by Descartes of Borg 950408
 */

#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOR;

static void create() {
    armor::create();
    SetKeyName("white glove");
    SetId( ({ "glove" }) );
    SetAdjectives( ({ "right", "white" ,"simple","elegant"}) );
    SetShort("a white glove");
    SetLong("A simple, yet elegant white glove with five fingers for the right hand.");
    SetDamagePoints(75);
    SetVendorType(VT_ARMOR);
    SetMass(10);
    SetDollarCost(10);
    SetArmorType(A_GLOVE);
    SetRestrictLimbs( ({"right hand"}) );
    SetProtection(BLUNT, 1);
    SetProtection(BLADE, 1);
    SetProtection(KNIFE, 2);
    SetProtection(HEAT, 3);
    SetProtection(COLD, 7);
    SetFingers(5);
}
void init(){
    ::init();
}
