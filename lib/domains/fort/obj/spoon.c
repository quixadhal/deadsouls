/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Mon Oct 17 23:40:14 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
#include <vendor_types.h>
inherit  LIB_ITEM;
void create(){
    ::create();
    SetKeyName("spoon");
    SetId();
    SetShort("a spoon");
    SetLong("A metal spoon.");
    SetVendorType(VT_TREASURE);
    SetMass(5);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetAdjectives("silver");
}
//funs
//snuf
void init() {
    ::init();
    SetSmell(([  "default" : "It has that weird smell of silver utensils."]));
}
//extras
