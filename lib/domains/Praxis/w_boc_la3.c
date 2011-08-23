#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 2);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort( "The western outskirts of Praxis");
    SetLong(
      "Beyond Praxis to the west tower the wicked Daroq Mountains which "
      "protect this part of reality from the punishing Qajip Desert.  "
      "Boc La Road of a local village leads east into the centre of the "
      "town.  West Road travels north from here.");
    SetExits( 
      (["north" : "/domains/Praxis/west_road1",
        "west" : "/domains/Praxis/highway1",
        "east" : "/domains/Praxis/w_boc_la2"]) );
    SetItems(
      (["road" : "Boc La Road runs east and west through Praxis. ",
        "roads" : "West Road and Boc La Road. ",
        "highway" : "A very heavily travelled highway to the western "
        "lands of Nightmare. Beware of the Daroq Mountains. ",
        "mountains" : "The Daroq Mountains.  Vile creatures are "
        "rumoured to live in and about them."]) );
    SetSkyDomain("town");
}
void init(){
    ::init();
}
