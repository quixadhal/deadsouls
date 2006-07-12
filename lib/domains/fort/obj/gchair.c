/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Mon Oct 17 20:56:22 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
#include <vendor_types.h>
inherit  LIB_CHAIR;
void create(){
    ::create();
    SetKeyName("chair");
    SetId("wooden chair","wood chair");
    SetShort("a wooden chair");
    SetLong("A simple chair, made of wood.");
    SetMaxSitters(1);
    SetMass(30);
    SetBaseCost(100);
}
//funs
//snuf
void init() {
    ::init();
}
//extras
