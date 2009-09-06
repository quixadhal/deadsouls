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
    SetShort("halway bend");
    SetLong("This is a bend in the hallway. The main Science Building hallway is north of here. A smaller halsway runs west from here.");
    AddExit("south", "/domains/campus/room/hazlab", (: PreExit :));
    SetClimate("indoors");
    SetItems( ([ 
        ({ "door","sliding door" }) : "A strange, metallic sliding door.",
      ]) );
    SetExits( ([
        "north" : "/domains/campus/room/science5.c",
      ]) );

}
void init(){
    ::init();
}
