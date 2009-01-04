#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(20);
    SetShort("stairwell");
    SetLong("You are standing on a stairwell landing. Flights of stairs "+
            "lead up and down, although the stairs up are roped off to prevent "+
            "passage. The flickering fluorescent lights make it difficult to see what "+
            "lies below here. There is a sign tied to the rope on the flight of stairs "+
            "going up.");
    SetItems(([
                ({"stairs","flight","flight of stairs"}) : "The stairs are made of concrete, appear to "+
                "be part of the foundation of the building, and seem quite sturdy.",
                "landing" : "This is a landing between flights of stairs leading up and down.",
                "rope" : "Thick yellow construction rope. There's a lot of it tied to the " +
                "flight of stairs leading up, preventing your passage. There's a sign hanging "+
                "on it.",
                ({"light","lights","fluorescent lights"}) : "Cheap lighting that doesn't seem to work "+
                "very well. Occasionally the landing is lit orange, then white again by the flickering "+
                "lights.",
                "sign" : "This is a handwritten, cardboard sign hanging on the roped-off stairs."]));
    SetExits(([
                "north" : "/domains/campus/room/corridor3",
                "down" : "/domains/campus/room/basement" 
                ]));
    SetDoor("north","/domains/campus/doors/top_stairs");
    SetProperty("no attack", 1);
}
void init(){
    ::init();
    add_action("r_sign","read");
    add_action("untie_r","untie");
}
int r_sign(string str){
    if(str=="sign" || str=="cardboard sign"){
        write("The sign reads:\n"+
                "WARNING! Second floor under construction, all access prohibited!\n");
        say(this_player()->GetName()+" read the sign.\n");
        return 1;
    }
}
int untie_r(string str){
    if(str=="rope" || str=="yellow rope"){
        write("The rope is quite firmly tied. You fail.\n");
        say(this_player()->GetName()+" fails to untie the rope.\n");
        return 1;
    }
}
