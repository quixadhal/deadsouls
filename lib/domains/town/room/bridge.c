#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Humpbacked bridge");
    SetLong("You are on an old, humpbacked bridge. Although the bridge has "
      "seen better days, the noticeable cracks in its "
      "surface seem to have in no way affected its "
      "stability. Some peculiar writing is scrawled on the bridge. "+
      "The bridge spans a small yet nonetheless formidable river. "
      "A town is visible east of here, and a dark forest looms west. "+
      "A small path winds down below the bridge to the riverbank.");
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
    SetExits( ([
        "east" : "/domains/town/room/road2",
        "west" : "/domains/town/room/forest_path1",
        "down" : "/domains/town/room/riverbank",
      ]) );
    SetRead(({"writing","scrawl","grafitti","grafitto"}),"HVMANES EVNT DOMVS");
    SetFlyRoom("/domains/town/virtual/sky/25,100000,1");
}
void init(){
    ::init();
}
