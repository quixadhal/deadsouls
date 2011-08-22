#include <lib.h>
inherit LIB_ROOM;

int eventHopDown(){
    write("You hop down into the hole, noting that there is no ladder for getting back up.");
    say(this_player()->GetName()+" hops into the open manhole.",this_player());
    return 1;
}
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(16);
    SetShort("access tunnel");
    SetLong("You are standing in what appears to be an underground "+
            "service access tunnel of some kind. The overhead neon lights flicker and "+
            "provide barely useful illumination. The air is damp and musty, and it "+
            "feels as though nobody has gone through here in years. The tunnel runs "+
            "south from here. This appears to be the north end of the tunnel. There "+
            "is a large, open manhole in the floor here.");
    SetItems(([
                ({"wall","walls","corridor","hall"}) : "The thick, metal walls here are scarred "+
                "and badly corroded. Wherever you are, it's seen heavy use in the distant past.",
                ({"lights","light","neon lights"}) : "Overhead neon lights. They appear on the "+
                "verge of failure.",
                ({"floor","ground"}) : "The floor is composed of metal plates welded together, "+
                "with raised traction cleats.",
                ({"cleat","cleats"}) : "Little raised bumps on the floor so you won't slip and fall.",
                ({"ladder","metal ladder"}) : "You see rusted bolts in the concrete floor and in "+
                "the hole that suggest a ladder was once here.",
                ({"hole","manhole"}) : "This is an opening built into the floor for access to "+
                "whatever it is that lies below. It looks like there was once a metal ladder for easy "+
                "movement, but it's now gone, so any trip down is one-way.",
                ]));
    AddExit("down","/domains/campus/room/sewer", (: eventHopDown :) );
    AddExit("south","/domains/campus/room/access5");
}
int CanReceive(object ob){
    if(ob && ob->GetRace() == "rodent"){
        message("info","You are repelled by rodenticide.",ob);
        return 0;
    }
    return ::CanReceive(ob);
}
void init(){
    ::init();
}
