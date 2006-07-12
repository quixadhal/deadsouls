/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Sun Oct 16 19:52:14 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
inherit  LIB_ARMOR;
void create(){
    ::create();
    SetKeyName("leather armor");
    SetId("suit","armor");
    SetShort("a suit of leather armor");
    SetLong("This is a suit made of strong, tough leather. While\nnot nearly as protective as metal, it permits a greater\nrange of flexibility.");
}
//funs
//snuf
void init() {
    ::init();
    SetSmell(([  "default" : "It smells rather nice. Still has that new armor smell."]));
    SetListen(([]));
}
//extras
