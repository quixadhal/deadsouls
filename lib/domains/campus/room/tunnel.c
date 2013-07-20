#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(5);
    SetShort("Tunnel");
    SetLong("You are in a dark and narrow underground tunnel. The walls are bare rock "+
            "and dirt...as if the tunnel has been carved out of and into the earth. Pipes and cables "+
            "run along the southern wall, and seem to emit a low hum. To the east you see a false wall "+
            "leading into the basement of the Vitual Campus administrative building. To the "+
            "west, the tunnel continues into darkness.");
    SetItems(([
                "tunnel" : "This is evidently a hidden tunnel beneath the Virtual Campus. It's "+
                "extremely creepy and dark in here.",
                "walls" : "The walls are bare earth and rocks. It looks as though whoever "+
                "built this tunnel was in a big hurry to get it done.",
                ({"wall","south wall","southern wall"}) : "The south wall is heavily laden with "+
                "thick cables and metal pipes running west along its length.",
                ({"rock","earth"}): "The walls are composed of this stuff...it's a rough excavation.",
                ({"pipe","pipes","cable","cables"}): "These thick pipes and cables appear to carry power somewhere along the wall. They are humming slightly.",
                "darkness":"It's deep. It's dark. It's the absence of light, and there's lots of it.",
                ]));
    SetListen("default","You hear faint echoes of dripping water.");
    SetExits(([
                "east" : "/domains/campus/room/basement2",
                "west" : "/domains/campus/room/tunnel2"]));
    SetProperty("no attack", 1);
    SetCoordinates("4003,4000,-1");
}
void init(){
    ::init();
    AddListen(({"pipe","pipes","wall","cables","cable"}) , "The pipes and "+
            "cables throb with some unknown power...emitting an eerie hum.");
}
