/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Tue Oct 18 00:46:53 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
#include <vendor_types.h>
inherit  LIB_STORAGE;
void create(){
    ::create();
    SetKeyName("chest");
    SetId("metal chest");
    SetShort("a metal chest");
    SetLong("This is a chest for storing things in.");
    SetVendorType(VT_TREASURE);
    SetMaxCarry (4000);
    SetMass(5000);
    SetBaseCost("silver",500);
    SetVendorType(VT_TREASURE);
    SetAdjectives("metal","heavy");
    SetCanClose(1);
    SetInventory(([  MY_AREA + "/" +"armor/chainmail" : 1 ,
	MY_AREA + "/" +"armor/f_platemail" : 1]));
}
//funs
//snuf
void init() {
    ::init();
}
//extras
