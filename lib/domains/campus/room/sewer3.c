#include <lib.h>
#include <damage_types.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(20);
    SetShort("service tube");
    SetLong("This is a narrow area that allows travel between the "+
            "surface above and the sewer below. It's cramped, dirty, and "+
            "not a friendly place for most humanoids. Some light seems to "+
            "be coming in from a drainage grate above.");
    SetItems( ([ ({"tube","service tube"}) : "You are in a service tube."+
                " It's foul, dark, and hot.",
                ({"sewer","sewer system"}) : "Though evidently in good repair "+
                "and of modern construction, this is still a sewer tunnel, "+
                "and it's hostile to human comfort.",
                "air" : "It is rank with the reek of decomposing waste.",
                "light" : "There isn't much of it, and you can't tell if it's " +
                "sunshine or lamplight, but it lets you see a bit better.",
                ]) );
    SetExits(([
                "down" : "/domains/campus/room/sewer2",
                "up" : "/domains/campus/room/south_road2",
                ]));
    SetSmell( ([ "default" : "The stench of sewage and waste hangs here."]) );
    SetListen("default","You hear faint echoes of dripping water.");
    SetDoor("up","/domains/campus/doors/grate");
}

void init(){
    ::init();
}
