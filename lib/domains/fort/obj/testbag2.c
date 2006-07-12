/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Tue Oct 18 00:05:06 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
#include <vendor_types.h>
inherit  LIB_STORAGE;
void create(){
    ::create();
    SetKeyName("testbag");
    SetId("bag");
    SetShort("a test bag");
    SetLong("A bag for testing.");
    SetMass(274);
    SetDollarCost(1);
    SetMaxCarry(50000);

    SetInventory(([  MY_ARMOR + "/helmet" : 1 ,
	//MY_OBJ + "/" +"pebble" : 1 ,
	//MY_ARMOR + "/" +"necklace" : 1 ,
	//MY_WEAP + "/" +"dagger" : 1 ,
	//MY_AREA + "/" +"weap/sharpsword" : 1
      ]));
}
//funs
//snuf
void init() {
    ::init();
}
//extras
