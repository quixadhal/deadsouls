#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort("West Boc La");
    SetDayLong( "Western Praxis is a more rural district of town.  "
      "The southern area opens up into farmlands while the northern "
      "side is lined with small huts and houses. The road continues "
      "to run in an east-west direction.");
    SetNightLong("Street lamps line the streets of rural West "
      "Praxis lighting the small huts and houses to the north and "
      "the farmlands to the south. To the east and west the road "
      "continues.");

    SetSmell("default", "Wisps of a magical smelling smoke are rising "
      "from a house on the north side of West Boc La.");
    SetSmell("smoke", "It smells unlike any substance you have ever "
      "encountered.");
    SetSmell("house", "It smells of the magic smoke coming from it.");
    SetExits( 
      (["east" : "/domains/Praxis/w_boc_la1",
        "west" : "/domains/Praxis/w_boc_la3",
        "south" : "/domains/Praxis/farm",
        "north" : "/domains/Praxis/sage_room"]) );
    SetItems(
      (["road" : "Boc La Road runs east and west through Praxis.",
        "roads" : "West Road and Boc La Road.",
        "highway" : "A very heavily travelled highway to the "
        "western lands of Nightmare. Beware of the Daroq Mountains.",
        "mountains" : "The Daroq Mountains.  Vile creatures are "
        "rumoured to live in and about them.",
        "house" : "This tiny little house has wisps of white "
        "smoke coming out of the door and windows.",
        "smoke" : "Lacey wisps of white smoke are coming out "
        "of the house to the north."]) );
    SetSkyDomain("town");
}
void init(){
    ::init();
}
