#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperty("light", 3);
    SetProperty("night light", 2);
    SetProperty("no castle", 1);
    SetShort("A small yard");
    SetDayLong( "Daytime makes this beautiful yard at the end of a "
      "small alley glow with peace. Quaint houses surround the yard "
      "on all flanks except to the west where the alley intersects "
      "Centre Path. There is a manhole where the alley meets the "
      "yard.");
    SetNightLong( "Very little stirs at the end of this small alley "
      "in the dark of the night. Some light illuminates the area from "
      "the lamps which are posted in front of the houses which "
      "surround the yard at the end of the alley.");

    SetDoor("/domains/Praxis/sewer_ent","down");
    "/domains/Praxis/sewer_ent"->SetOpen("manhole", 0);
    SetExits( 
      (["west" : "/domains/Praxis/s_centre2",
        "north" : "/domains/Praxis/sheriff",
        "down" : "/domains/Praxis/sewer_ent"]) );
    SetItems(
      (["alley" : "It looks beautiful here.",
        ({ "lamp", "lamps" }) : "Dimly illuminated lights.",
        "manhole" : (: this_object(), "look_at_manhole" :) ]) );
    SetSkyDomain("town");
    SetInventory(([
        "/domains/Praxis/obj/mon/beggar" : 1,
      ]));
}

void reset() {
    ::reset();
    if(!find_living("beggar") || !environment(find_living("beggar")))
        new("/domains/Praxis/obj/mon/beggar")->move(this_object());
    if(query_reset_number() != 1) {
        "/domains/Praxis/sewer_ent"->SetOpen("manhole", 0);
    }
}
void init(){
    ::init();
}
