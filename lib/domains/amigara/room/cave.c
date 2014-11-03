#include <lib.h>
#include <climb.h>
#include ROOMS_H

inherit LIB_ROOM;
inherit LIB_CLIMB;

int DownClimb(){
    tc("hit DownClimb");
    eventClimb(this_player(),CLIMB_DOWN,"/domains/amigara/room/passage1");
}

void create() {
    ::create();
    SetAmbientLight(5);
    SetNightLight(15);
    SetDayLight(25);
    SetShort("A Dark Cave");
    SetNightLong("This is a cave in the rough stone mountain wall. "+
            "The cave opens south into the night. To the north is darkness.");
    SetDayLong("This is a cave in the rough stone mountain wall. Daylight shines in from the south, where where a forest lies. To the north, the cave continues into darkness.");
    SetLong("This is a small cave near a forest.");
    SetItems(([
                ({ "wall", "rocky wall", "mountain wall", "rough stone mountain wall", "stone mountain wall" }) : "Geological activity has created this cave ove the years, but clearly some sentient activity has formed it into a usable size and shape for habitation.",
                ]));
    SetInventory(([
                "/domains/town/obj/ladder" : 1,
                ]));
    SetExits( ([
                "south" : "/domains/town/virtual/forest/-3,25",
                "north" : "/domains/amigara/room/midair.c",
                ]) );
    SetClimb( (: DownClimb :) , CLIMB_DOWN);
    SetObviousExits("south, climb down");
}
void init(){
    ::init();
}
