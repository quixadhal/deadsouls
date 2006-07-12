/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Mon Oct 17 21:00:03 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
#include <vendor_types.h>
inherit  LIB_CHAIR;
void create(){
    ::create();
    SetKeyName("stool");
    SetId("simple stool");
    SetShort("a short stool");
    SetLong("A simple stool for sitting on, made of wood.");
    SetMaxSitters(1);
    SetMass(10);
    SetBaseCost(30);
    SetAdjectives("short","simple","wood","wooden");
}
//funs
//snuf
void init() {
    ::init();
    SetSmell(([  "default" : "Dude. That's gross."]));
}
//extras
