#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(5);
    SetShort("Tunnel");
    SetLong("You are in a dark and narrow underground tunnel. The walls are bare rock "+
            "and dirt...as if the tunnel has been carved out of and into the earth. Pipes and cables "+
            "run along the southern wall, and seem to emit a low hum. To the west and "+
            "east, you can see some light.");
    SetListen("default","You hear faint echoes of dripping water.");
    SetItems(([
                "tunnel" : "This is evidently a hidden tunnel beneath the Virtual Campus. It's "+
                "extremely creepy and dark in here.",
                "walls" : "The walls are bare earth and rocks. It looks as though whoever "+
                "built this tunnel was in a big hurry to get it done.",
                ({"wall","south wall","southern wall"}) : "The south wall is heavily laden with "+
                "thick cables and metal pipes running along its length.",
                ({"rock","earth"}): "The walls are composed of this stuff...it's a rough excavation.",
                "darkness":"It's deep. It's dark. It's the absence of light, and there's lots of it.",
                ]));
    SetExits(([
                "east" : "/domains/campus/room/tunnel",
                "west" : "/domains/campus/room/access7"]));
    SetProperty("no attack", 1);
}
void init(){
    ::init();
    AddListen(({"pipe","pipes","wall","cables","cable"}) , "The pipes and "+
            "cables throb with some unknown power...emitting an eerie hum.");
}
