/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Mon Oct 17 23:02:51 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
#include <vendor_types.h>
inherit  LIB_SURFACE;
void create(){
    ::create();
    SetKeyName("table");
    SetId("wood table");
    SetShort("a wooden table");
    SetLong("A wooden table.");
    SetVendorType(VT_TREASURE);
    SetMaxCarry (5000);
    SetMass(5500);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetAdjectives("wood","wooden");
    SetInventory(([  MY_OBJ + "/" +"pebble" : 1,
	MY_OBJ + "/" +"cup" : 1,
	MY_OBJ + "/" +"spoon" : 1
      ]));
}


void init() {
    ::init();
}
