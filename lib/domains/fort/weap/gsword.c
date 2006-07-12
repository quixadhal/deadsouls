/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Mon Oct 17 21:05:53 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
#include <vendor_types.h>
#include <damage_types.h>
inherit  LIB_ITEM;
void create(){
    ::create();
    SetKeyName("sword");
    SetId("normal sword");
    SetShort("a sword");
    SetLong("This is a typical sword of the Fortress Guard. Nothing fancy, just something to kill with.");
    SetWeaponType("blade");
    SetVendorType(VT_WEAPON);
    SetClass(30);
    SetDamageType(BLADE);
    SetMass(100);
    SetBaseCost("silver",500);
    SetAdjectives("typical");
}
//funs
//snuf
void init() {
    ::init();
}
