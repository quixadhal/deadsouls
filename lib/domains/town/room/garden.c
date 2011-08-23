#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetNightLight(10);
    SetDayLight(30);
    SetShort("Mansion Garden");
    SetLong("This is what is left of what was once a beautiful, "
      "if simple, garden. It is now badly overgrown with "
      "weeds, and it is clear that foraging animals long "
      "ago ate the delicate flowers and vegetables that "
      "once grew here. The gardener's shack is north.");
    SetItems(([
        ({ "flowers", "vegetables", "flowers and vegetables" }) : "If they were once here, they certainly no longer are.",
        ({ "shack", "wooden shack" }) : "A beaten-up old wooden shack is north.",
        "garden" : "A mess of weeds and dirt.",
      ]));
    SetSkyDomain("town");
    SetExits( ([
        "east" : "/domains/town/room/mansion_ext",
        "north" : "/domains/town/room/shack"
      ]));
    SetEnters( ([
        "shack" : "/domains/town/room/shack",
      ]) );
}
void init(){
    ::init();
}
