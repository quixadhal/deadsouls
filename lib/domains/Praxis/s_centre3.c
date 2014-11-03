#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort("Southern Praxis");
    SetDayLong( "Southern Praxis is the business district of this "
            "small adventuring community.  Through it runs Centre Path, "
            "bringing people from the centre of town through here and "
            "to the southwest the area really begins to rumble with "
            "horse and foot traffic.");
    SetNightLong( "The business district of Praxis nearly slumbers "
            "save for a few nocturnal races of adventurer.  Centre Path "
            "winds down from the north out to the Port of Praxis southwest. "
            " An unnamed road heads east.");
    SetExits( 
            (["north" : "/domains/Praxis/s_centre2",
             "southwest" : "/domains/Praxis/s_centre4",
             "east" : "/domains/Praxis/unnamed1"]) );
    SetItems( 
            ([({"path", "centre path"}) : "The main road leading north and "
             "south through Praxis.",
             ({"port", "port of praxis"}) : "You cannot see that from here.", 
             ({"road", "unnamed road"}) : "It leads out of town."]) );
    SetSkyDomain("town");
    SetListen("road", "An eerie silence fills the air that way.");
}
void init(){
    ::init();
}
