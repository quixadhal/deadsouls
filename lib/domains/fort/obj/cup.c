/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Mon Oct 17 23:37:21 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
#include <vendor_types.h>
inherit  LIB_ITEM;
void create(){
    ::create();
    SetKeyName("cup");
    SetId();
    SetShort("a cup");
    SetLong("A tin cup designed to contain liquids. It appears broken and unusable.");
    SetVendorType(VT_TREASURE);
    SetMass(10);
    SetVendorType(VT_TREASURE);
    SetAdjectives("tin");
    SetNoCondition(1);
}
//funs
//snuf
void init() {
    ::init();
}
//extras
