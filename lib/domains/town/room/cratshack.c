#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("north of house");
    SetLong("The thick woods give way to a small clearing here, where an old house, painted white, stands. The front porch of the house is south. A narrow trail leads into the woods to the north.");
    SetItems(([
        ({ "forest", "wood", "vegetation" }) : "All around is vegetation growing so incredibly dense that it is impossible to wander off the path.",
        ({ "porch", "front porch" }) : "The wooden front porch of an old white house.",
        ({ "trail", "path" }) : "To the north, a narrow path leads into the woods.",
        ({ "clearing", "woods" }) : "As if by powerful magic, the woods clear here, where the house stands.",
        ({ "house", "white house", "old house" }) : "The house looks weatherbeaten, ill maintained, and rather old. Its construction is of an archaic design no longer used. Despite its abused appearance, the house seems to have a quiet dignity about it, as if many people had been its friends over the years, and it simply waited for their return.",
      ]));
    SetSkyDomain("town");
    SetExits( ([
        "north" : "/domains/town/room/narrow_path2",
        "south" : "/domains/town/room/porch.c",
      ]) );
    SetInventory(([
        "/domains/town/obj/mailbox" : 1,
      ]));

}
void init(){
    ::init();
}

int CanReceive(object ob){
    if(answers_to("provost",ob)) return 0;
    return ::CanReceive();
}
