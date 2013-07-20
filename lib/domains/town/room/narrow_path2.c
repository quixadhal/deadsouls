#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("a narrow path");
    SetLong("This is a narrow path running north and south through a dense forest. The thick vegetation makes travel off the path impossible.");
    SetItems(([
        ({ "path", "trail" }) : "A very narrow path that seems threatened all around by the looming woods. It's hard to imagine what force might have cut this thin trail into the vegetation here.",
        ({ "forest", "wood", "vegetation" }) : "All around is vegetation growing so incredibly dense that it is impossible to wander off the path.",
      ]));
    SetSkyDomain("town");
    SetExits( ([
        "north" : "/domains/town/room/narrow_path",
        "south" : "/domains/town/room/cratshack.c",
      ]) );

    SetInventory(([
      ]));

}
void init(){
    ::init();
}

int CanReceive(object ob){
    if(answers_to("provost",ob)) return 0;
    return ::CanReceive();
}
