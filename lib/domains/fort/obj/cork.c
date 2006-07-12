/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Sun Oct 16 19:40:27 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
inherit  LIB_ITEM;
void create(){
    ::create();
    SetKeyName("cork");
    SetId("wine cork");
    SetShort("a cork");
    SetLong("This is a typical cork, small and cylindrical, for \nplugging wine bottles.");
}
//funs
//snuf
void init() {
    ::init();
    SetSmell(([  "default" : "It smells a bit like wine."]));
    SetListen(([]));
}
//extras
