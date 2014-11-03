#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort( "Sun Alley near East Road");
    SetLong(
            "South of here, at the corner of this sunny alley is the Last "
            "Dragon Restaurant. The building is a brownish grey wooden structure "
            "with very little adornment on the outside. East Road interesects with "
            "this alley over to the east. The beginnings of a forest indicate "
            "the borders of Praxis here.");
    SetItems(
            (["foo" : "Whatever you say.",
             "alley" : "This alley is the northernmost part of Praxis. "
             "Only the great North Forest is north of here.",
             "restaurant" : "A great place to rest up from adventuring.",
             "road" : "East Road.",
             "forest" : "The huge forest north of Praxis.  The "
             "walled town of Cartesia is out there somewhere.",
             "border" : "The forest keeps Praxis from expanding any "
             "further north."]) );
    SetSkyDomain("town");
    SetExits( 
            (["east" : "/domains/Praxis/east_road2",
             "west" : "/domains/Praxis/sun1",
             "south"	 : "/domains/Praxis/restaurant"]) );
}
void init(){
    ::init();
}
