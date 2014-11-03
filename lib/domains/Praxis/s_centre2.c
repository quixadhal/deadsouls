#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 2);
    SetProperty("no castle", 1);
    SetShort( "Centre Path just south of Krasna Square");
    SetLong(
            "A small alley crosses Centre Path here just south of Krasna Square.  "
            "As it winds on south, Centre Path travels through the business "
            "center of South Praxis up to the Port of Praxis.");
    SetExits( 
            (["north" : "/domains/Praxis/s_centre1",
             "south" : "/domains/Praxis/s_centre3",
             "east" : "/domains/Praxis/yard",
             "west" : "/domains/Praxis/alley1"]) );
    SetItems(
            ([({ "path", "road", "centre path" }) : "Centre Path leads to "
             "Monument Square north and to the port south.",
             "alley" : "It opens up to a yard of houses east, and a post "
             "and bank west."]) );
    SetSkyDomain("town");
}
void init(){
    ::init();
}
