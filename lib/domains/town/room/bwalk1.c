#include <lib.h>
#include <daemons.h>

inherit LIB_FISHING;

varargs string readSign() {
    string ret = "WARNING! Orcs, dwarves, and some other races cannot "+
        "swim! If you jump into the sea you will sink and drown!";
    return ret;
}

static void create() {
    fishing::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Dock of the Bay");
    SetNightLong("This is an area comprised of a raised wooden platform over the beach that allows folks to walk around enjoying the view and perhaps visiting the local commercial establishments. To the west is Harry's Pub. North is the town dock. A great sea stretches out into the eastern horizon. A lone lamp post lights this area. \n%^GREEN%^There is a sign here you can read.%^RESET%^");
    SetDayLong("This is an area comprised of a raised wooden platform over the beach that allows folks to walk around enjoying the view and perhaps visiting the local commercial establishments. To the west is Harry's Pub. North is the town dock. A great sea stretches out into the eastern horizon. \n%^GREEN%^There is a sign here you can read.%^RESET%^");
    SetItems( ([
                "establishment":"A pub west of here.",
                "local establishment":"A pub west of here.",
                "commercial establishment":"A pub west of here.",
                "sign":"A sign on the ground. To read it, 'read sign'.",
                "platform":"The local surface.",
                "raised wooden platform":"The local surface.",
                "wooden platform":"The local surface.",
                "raised platform":"The local surface.",
                "horizon":"Where the sky loves the sea.",
                "beautiful horizon":"Where the sky loves the sea.",
                "town":"A population center to the northwest.",
                "boardwalk":"Where people can stably walk and enjoy the view.",
                "view":"A beautiful horizon to the east.",
                "dock":"What boats do north of here.",
                "shoreline":"The demarcator of discontinuity between land and sea.",
                ]) );
    SetExits( ([
                "north" : "/domains/town/room/docks",
                "west" : "/domains/town/room/hp.c",
                ]) );
    SetRead("sign", (: readSign :) );
    SetFrequency(5);
    SetChance(90);
    SetFish( ([
                "/domains/town/meals/shark" : 10,
                "/domains/town/meals/herring" : 2,
                ]) );
    SetActionsMap( ([ 
                "A soft breeze cools your brow." : 5,
                "You briefly hear seagulls far in the distance." : 7,
                "A cool breeze flows in from the east, " +
                "bringing with it the bracing salty smells of the sea.": 2,
                ]) );
    SetInventory( ([
                "/domains/town/obj/seawater" : 1,
                ]) );
    AddItem(new("/domains/town/obj/lamp"));
    SetSkyDomain("town");
}

void init(){
    ::init();
    add_action("enjoy","enjoy");
}

int enjoy(string foo){
    write("Mmmm!");
    return 1;
}
