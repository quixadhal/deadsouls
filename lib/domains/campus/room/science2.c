#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

int PreExit(mixed args...){
    object who = this_player();
    write("You are scanned by a beam of light from the sliding door.");
    say(who->GetName()+" is scanned by a beam of light from the sliding door.");
    if(!present("pass of dr kleiner",who)){
        write("The door does not open.");
        return 0;
    }
    else {
        write("The door opens, letting you through.\n");
        say("The door lets "+who->GetName()+" through.\n");
        return 1;
    }
} 

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Science Building Hallway, West");
    SetLong("This is the western end of the main hallway in the LPC University Science Building. North of here is the stargate laboratory. The hallway runs east from here, toward the building's exit. To the south is a sliding door.");
    SetExits( ([
                "north" : "/domains/campus/room/slab",
                "east" : "/domains/campus/room/science1",
                ]) );
    AddExit("south", "/domains/campus/room/hazlab", (: PreExit :));
    SetClimate("indoors");
    SetItems( ([ 
                ({ "door","sliding door" }) : "A strange, metallic sliding door.",
                ]) );
}
void init(){
    ::init();
}
