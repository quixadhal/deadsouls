#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort("East Boc La south of the monastary");
    SetLong(
      "Boc La Road on the eastern side of Praxis is a large dirt road "
      "leading towards the center of town, Monument Square. Many "
      "footprints have pounded the road into a flat surface, easy to "
      "travel. Rusted old lamp posts stand on either side of the road, "
      "to light Praxis by night. A small path to the north leads into the "
      "village monastary. East Boc La road continues east from here, "
      "and the town hall is south.");
    SetExits( 
      (["north" : "/domains/Praxis/monastery",
        "east" : "/domains/Praxis/e_boc_la2",
        "south" : "/domains/Praxis/town_hall.c",
        "west" : "/domains/Praxis/square"]) );
    SetItems(
      (["road" : "Boc La Road is the large path that runs east-west "
        "through Praxis.", 
        "monastery" : "The monks of Nightmare study and "
        "worship there.", 
        "square" : "Monument Square is the center point of "
        "all of Praxis. Many adventurers enjoying hanging out there, "
        "chatting with fellow citizens about life in Praxis."]) );
    SetSkyDomain("town");
}
void init(){
    ::init();
}
