/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Sun Oct 16 20:12:41 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include "../customdefs.h";
inherit  LIB_ARMOR;
void create(){
    ::create();
    SetKeyName("beret");
    SetId("hat","red beret");
    SetShort("a red beret");
    SetLong("This red beret bears the proud insignia of the Fortress Guards.");
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
