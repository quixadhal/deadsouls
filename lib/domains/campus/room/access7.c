#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(16);
    SetShort("access tunnel");
    SetLong("You are standing in what appears to be an underground "+
            "service access tunnel of some kind. The overhead neon lights flicker and "+
            "provide barely useful illumination. The air is damp and musty, and it "+
            "feels as though nobody has gone through here in years. The tunnel runs "+
            "north and south from here.");
    SetItems(([
                ({"wall","walls","corridor","hall"}) : "The thick, metal walls here are scarred "+
                "and badly corroded. Wherever you are, it's seen heavy use in the distant past.",
                ({"lights","light","neon lights"}) : "Overhead neon lights. They appear on the "+
                "verge of failure.",
                ({"floor","ground"}) : "The floor is composed of metal plates welded together, "+
                "with raised traction cleats.",
                ({"cleat","cleats"}) : "Little raised bumps on the floor so you won't slip and fall.",
                ]));
    SetEnters( ([ 
                ]) );
    AddExit("north","/domains/campus/room/access6");
    AddExit("south","/domains/campus/room/access9");
    AddExit("east","/domains/campus/room/tunnel2");
}

void init(){
    ::init();
}
