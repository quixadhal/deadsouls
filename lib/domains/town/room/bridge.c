#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Humpbacked bridge");
    SetNightLong("This is an old, humpbacked bridge, lit by a lamp post. It's seen better days, though it appears still quite stable enough. Some peculiar writing is scrawled on the bridge. A town is east of here. To the west is darkness. There is an unlit path leading down below the bridge.");
    SetDayLong("This is an old, humpbacked bridge. It's seen better days, though it appears still quite stable enough. Some peculiar writing is scrawled on the bridge. The bridge spans a river below. A town is east of here, and a dark forest looms west. There is a small path leading down below the bridge.");
    SetItems( ([
        ({"cracks","surface","bridge"}) : "The bridge "
        "surface has a few cracks, but they seem old "
        "and minor...mostly signs of how long the "
        "structure has withstood the the elements.",
        ({"span","river","stream","bank"}) : "Below "
        "you is a small river with a powerful, "
        "swift current. You might be able to climb "
        "down the bridge to get to the riverbank "
        "below.",
        "town" : "A small town lies east of here.",
        ({"writing","scrawl","grafitti","grafitto"}):
        "Grafitti is written here, in an odd and "
        "uncertain hand, as if unused to the effort "
        "of writing.",
        ({"path","forest","dark forest"}) : "West of the "
        "bridge is a path going into a dark forest."
      ]) );
    SetSkyDomain("town");
    SetExits( ([
        "east" : "/domains/town/room/road2",
        "west" : "/domains/town/room/forest_path1",
        "down" : "/domains/town/room/riverbank",
      ]) );
    SetRead(({"writing","scrawl","grafitti","grafitto"}),"HVMANES EVNT DOMVS");
    AddItem(new("/domains/town/obj/lamp"));
}
void init(){
    ::init();
}
