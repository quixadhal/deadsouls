#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("front porch");
    SetLong("This is the rickety front porch of an old house. The front door is south. To the north is a clearing in the woods, and a path leading away.");
    SetItems(([
                ({ "forest", "wood", "vegetation" }) : "All around is vegetation growing so incredibly dense that it is impossible to wander off the path.",
                ({ "porch", "front porch" }) : "The wooden front porch of an old white house.",
                ({ "trail", "path" }) : "To the north, a narrow path leads into the woods.",
                ({ "clearing", "woods" }) : "As if by powerful magic, the woods clear here, where the house stands.",
                ({ "house", "white house", "old house" }) : "The house looks weatherbeaten, ill maintained, and rather old. Its construction is of an archaic design no longer used. Despite its abused appearance, the house seems to have a quiet dignity about it, as if many people had been its friends over the years, and it simply waited for their return.",
                ]));
    SetInventory(([
                "/domains/town/obj/rocking_chair" : 2,
                ]));
    SetExits( ([
                "north" : "/domains/town/room/cratshack",
                "south" : "/domains/town/room/living_room.c",
                ]) );

    SetDoor("south", "/domains/town/doors/house_door.c");

}
void init(){
    ::init();
}
int CanReceive(object ob){
    if(answers_to("provost",ob)) return 0;
    return ::CanReceive();
}
