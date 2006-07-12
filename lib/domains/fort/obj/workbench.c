/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Mon Oct 17 23:14:24 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
#include <vendor_types.h>
inherit  LIB_SURFACE;
void create(){
    ::create();
    SetKeyName("bench");
    SetId("workbench","bench");
    SetShort("a workbench");
    SetLong("This is the wooden workbench of some kind of designer or builder. It is burned, pitted, stained, and otherwise heavily abused.");
    SetVendorType(VT_TREASURE);
    SetMaxCarry (2000);
    SetMass(2500);
    SetBaseCost("silver",200);
    SetVendorType(VT_TREASURE);
    SetAdjectives("wood","wooden","work");
    SetInventory(([]));
}
//funs
//snuf
void init() {
    ::init();
}
//extras
