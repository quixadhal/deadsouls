#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort( "Centre Path north of Krasna Square");
    SetLong("Centre Path is a highly-traveled dirt path that runs north and south through Praxis.  Just south of here, it intersects Boc La Road in Krasna Square.  A small church is off to the west. It is a medium sized building, made out of sturdy wood that has been painted white. A very simple, yet beautiful stain glass window stares out at you from the front of the church. The doors are a deep black, and one is slightly open, in welcome. To the east is the local hotel.");
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetItems(
            (["path" : "Centre Path leads to Monument square south and out "
             "of town north.",
             "road" : "Boc La Road is the main east-west road.",
             "church" : "It is a small church run by the local clerics.",
             "square" : "The central square of Praxis."]) );
    SetSkyDomain("town");
    SetExits( ([
                "south" : "/domains/Praxis/square",
                "west" : "/domains/Praxis/cleric_join",
                "north" : "/domains/Praxis/n_centre2",
                "east" : "/domains/Praxis/hotel.c",
                ]) );
}
void init(){
    ::init();
}
