#include <lib.h>
inherit LIB_ROOM;
inherit "/lib/props/ambiance";
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
    //\n%^GREEN%^There is a small sign "+  
    // "you can read, posted over a porthole set in the wall.");
    SetItems(([
                ({"wall","walls","corridor","hall"}) : "The thick, metal walls here are scarred "+
                "and badly corroded. Wherever you are, it's seen heavy use in the distant past.",
                ({"lights","light","neon lights"}) : "Overhead neon lights. They appear on the "+
                "verge of failure.",
                ({"floor","ground"}) : "The floor is composed of metal plates welded together, "+
                "with raised traction cleats.",
                ({"cleat","cleats"}) : "Little raised bumps on the floor so you won't slip and fall.",
                ({"hole","port","porthole","access port"}) : "An exit set in the wall, providing maintenance access.",
                ({"sign"}) : "A small, stenciled sign with writing on it.",
                ]));
    SetRead("sign","Campus domain special room access port: FREEZER");
    SetEnters( ([
                ({"hole","port","porthole","access port"}):"/domains/campus/room/freezer"
                ]) );
    AddExit("north","/domains/campus/room/access0");
    AddExit("south","/domains/campus/room/access4");
    SetObviousExits("n,s");
}
