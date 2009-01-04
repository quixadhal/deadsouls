#include <lib.h>
inherit LIB_ROOM;
inherit "/lib/props/ambiance";

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(16);
    SetShort("access tunnel");
    SetLong("You are standing in what appears to be an underground service access tunnel of some kind. The overhead neon lights flicker and provide barely useful illumination. The air is damp and musty, and it feels as though nobody has gone through here in years. The tunnel runs north from here.");
    //"\n%^GREEN%^There is a small sign "+  
    //     "you can read, posted over a porthole set in the wall.");
    SetItems(([
                ({"wall","walls","corridor","hall"}) : "The thick, metal walls here are scarred "+
                "and badly corroded. Wherever you are, it's seen heavy use in the distant past.",
                ({"lights","light","neon lights"}) : "Overhead neon lights. They appear on the "+
                "verge of failure.",
                ({"floor","ground"}) : "The floor is composed of metal plates welded together, "+
                "with raised traction cleats.",
                ({"cleat","cleats"}) : "Little raised bumps on the floor so you won't slip and fall.",
                ({"field","force field"}) : "A sheet-like, multicolored expression of energy.",
                ({"doorway","shimmering doorway"}) : "It looks like a force field restricts further "+
                "movement south.",
                ({"hole","port","porthole","access port"}) : "An exit set in the wall, providing maintenance access.",
                ({"sign"}) : "A small, stenciled sign with writing on it.",
                ]));
    SetEnters( ([ 
                ]) );
    SetInventory(([
                "/domains/campus/obj/labkey" : 1,
                ]));
    SetExits( ([ 
                "north" : "/domains/campus/room/access7",
                ]) );
    SetRead("sign","Campus domain special room access port: DEATH");
}
void init(){
    ::init();
}
