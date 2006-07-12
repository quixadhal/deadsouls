/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Sun Oct 16 19:46:57 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
inherit  LIB_ITEM;
void create(){
    ::create();
    SetKeyName("rock");
    SetId("small rock");
    SetShort("a small rock");
    SetLong("A small rock with a rather rough surface.");
}
//funs
//snuf
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
//extras
