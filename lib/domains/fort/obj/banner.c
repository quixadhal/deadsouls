/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Wed Oct 19 19:33:53 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
#include <vendor_types.h>
inherit  LIB_ITEM;
void create(){
    ::create();
    SetKeyName("banner");
    SetId("flag","banner","pole","standard");
    SetShort("The proud banner of the Fortress on the Frontiers");
    SetLong("This is the bold, crimson banner of the Fortress on the Frontiers, attached to a pole 10 feet high in the manner of a flag. It stands here proudly, seeming to claim this area as property of the Fortress.");
    SetVendorType(VT_TREASURE);
    SetMass(20);
    SetBaseCost(1);
    SetValue(10);
    SetVendorType(VT_TREASURE);
    SetAdjectives("proud","crimson","fortress","fort");
    SetItems(([  ({"base", "plate", "platelike base"}) : "The base on which the banner stands." ,
	({"pole", "standard", "flag", "stick"}) : "The banner is attached to the pole, which stands on a platelike base."]));
    SetNoCondition(1);
}

mixed CanGet() { return "The banner's pole is firmly attached to the ground and does not budge."; }

void init() {
    ::init();
}
