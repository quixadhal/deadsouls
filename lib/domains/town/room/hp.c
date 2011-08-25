#include <lib.h>

inherit LIB_ROOM;

varargs string readMenu() {
    string ret = "\tStein of Guinness\t\t\t1 gold\n"+
    "\tBottle of wine\t\t\t\t2 gold\n"+
    "\tBottle of whiskey\t\t\t3 gold\n";
    return ret;
}

void create(){
    ::create();
    SetAmbientLight(30);
    SetShort("Harry's Pub");
    SetLong("Harry's Pub is a simple place where lonely sailors pass the time away and talk about their homes. Known to serve some mean drinks and customers, the place is clean and spare, with few things to break over someone's head. A menu on the wall lists the available refreshments. There appears to be some kind of back room to the west.");
    SetClimate("indoors");
    SetItems( ([
        ({"wall", "walls"}) : "If only they could speak, eh?",
        "menu" : "A menu you can read. Oddly it has as its logo a severed "+
        "wolf's head on a stick.",
        ({"room","back room"}) : "Perhaps a storage area.",
        "harry" : "Nobody's seen Harry since the jive ring thing.",
      ]) );
    SetSkyDomain("town");
    SetRead("menu", (: readMenu :) );
    SetInventory(([
        "/domains/town/npc/brandy" : 1,
      ]) );
    SetExits( ([
        "east" : "/domains/town/room/bwalk1.c",
      ]) );
}

void init(){
    ::init();
}
