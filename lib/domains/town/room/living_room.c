#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("living room");
    SetLong("This is the living room of the old house. It is a small, modest home, apparently constructed with an eye toward practicality rather than fanciness. A doorway north leads to the front porch.");
    SetItems(([
                ({ "forest", "wood", "vegetation" }) : "All around is vegetation growing so incredibly dense that it is impossible to wander off the path.",
                ({ "porch", "front porch" }) : "The wooden front porch of an old white house.",
                ({ "trail", "path" }) : "To the north, a narrow path leads into the woods.",
                ({ "clearing", "woods" }) : "As if by powerful magic, the woods clear here, where the house stands.",
                ({ "house", "white house", "old house" }) : "The house looks weatherbeaten, ill maintained, and rather old. Its construction is of an archaic design no longer used. Despite its abused appearance, the house seems to have a quiet dignity about it, as if many people had been its friends over the years, and it simply waited for their return.",
                ]));
    SetInventory(([
                "/domains/town/obj/sofa" : 1,
                "/domains/town/obj/simple_chair" : 2,
                ]));
    SetExits(([
                "north" : "/domains/town/room/porch",
                ]));

    SetDoor("north", "/domains/town/doors/house_door.c");

}
void init(){
    ::init();
}
int CanReceive(object ob){
    if(answers_to("provost",ob)) return 0;
    return ::CanReceive();
}
