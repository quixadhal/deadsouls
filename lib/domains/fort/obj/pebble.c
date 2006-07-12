/* 
File created with thingmaker 
written by Cratylus@Frontiers 
but mostly stolen from Haderach's roommaker
Thing made by Cratylus
Date: Sun Oct 16 19:45:30 2005
WARNING! WARNING! If you edit this file by hand the thing maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
inherit  LIB_ITEM;
void create(){
    ::create();
    SetKeyName("pebble");
    SetId("flat pebble");
    SetShort("a small, flat pebble");
    SetLong("This is a smooth, flat pebble.");
}
//funs
//snuf
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
//extras
