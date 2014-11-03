#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 1);
    SetProperty("no castle", 1);
    SetShort( "West Boc La in Praxis");
    SetDayLong("The local pub on the southwest corner of the square "
            "east of here gets little business at the height of "
            "the day.  As twilight fall into dusk, however, you "
            "will notice more people making their way into the pub "
            "to cap off a day of adventure.  The road you are on, "
            "West Boc La, leads into the western districts of "
            "Praxis and east to the centre of town.");
    SetNightLong("The street lamps provide you with barely enough "
            "light to see, but can you can just see Krasna Square east and "
            "Lars' Pub to the south.  You can also make out a chapel to "
            "the north, but you see no entrance.");

    SetListen("default", "Cheers of local adventurers "
            "resonate from inside the pub.");
    SetExits( 
            (["east" : "/domains/Praxis/square",
             "west" : "/domains/Praxis/w_boc_la2",
             "south" : "/domains/Praxis/pub"]) );
    SetItems( 
            (["church" : "It looks like the entrance might be on the "
             "road leading north from Monument Square.",
             ({"lamps", "lamp", "street lamp"}) : "All of the street "
             "lamps along the road provide minimal light at night.",
             ({"pub", "lars pub", "lars' pub"}) : "Local adventurers "
             "wind down their day in Lars' Pub.",
             ({"road", "west boc la", "boc la"}) : "It goes beyond the "
             "western districts of Praxis into the mountains.",
             "square" : "At the centre of Praxis, it is where "
             "Boc La and Centre Path meet."]) );
    SetSkyDomain("town");
}
void init(){
    ::init();
}
