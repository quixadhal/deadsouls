/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Sun Oct 16 20:20:35 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
#include <vendor_types.h>
#include <damage_types.h>
#include <armor_types.h>
inherit  LIB_ARMOR;
void create(){
    ::create();
    SetKeyName("sun hat");
    SetId("hat","sunhat");
    SetShort("a floppy sun hat");
    SetLong("This light, wide-brimmed chapeau should help keep\nthe sun off your delicate skin.");
    SetArmorType(A_HELMET);
}
//funs
//snuf
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
//extras
