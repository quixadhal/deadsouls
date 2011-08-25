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
    SetNightLong("This is an area comprised of a raised wooden platform that extends into the water so that folks with boats can conveniently enter and exit their vehicles. The dock runs south along the shoreline, turning into a boardwalk. The town shore is north. A great sea stretches out into the eastern horizon. A lone lamp post lights this area. \n%^GREEN%^There is a sign here you can read.%^RESET%^");
    SetDayLong("This is an area comprised of a raised wooden platform that extends into the water so that folks with boats can conveniently enter and exit their vehicles. The dock runs south along the shoreline, turning into a boardwalk. The town shore is north. A great sea stretches out into the eastern horizon. \n%^GREEN%^There is a sign here you can read.%^RESET%^");
    SetItems( ([
        "sign":"A sign on the ground. To read it, 'read sign'.",
        "platform":"The local surface.",
        "raised wooden platform":"The local surface.",
        "wooden platform":"The local surface.",
        "raised platform":"The local surface.",
        "horizon":"Where the sky loves the sea.",
        "town":"A population center to the northwest.",
        "boardwalk":"Leads south from here.",
        "dock":"What boats do here.",
        "shoreline":"The demarcator of discontinuity between land and sea.",
      ]) );
    SetExits( ([
        "north" : "/domains/town/room/shore",
        "south" : "/domains/town/room/bwalk1.c",
      ]) );
    SetProperty("dock", 1);
    SetRead("sign", (: readSign :) );
    SetFrequency(5);
    SetChance(90);
    SetFish( ([
        "/domains/town/meals/shark" : 10,
        "/domains/town/meals/herring" : 20,
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
}
