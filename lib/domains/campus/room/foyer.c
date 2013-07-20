#include <lib.h>
inherit LIB_ROOM;


int readSign() {
    this_player()->more("/domains/campus/txt/warning.txt");
    return 1;
}
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(40);
    SetShort("Foyer");
    SetLong("This is the west end of the main hallway "+
            "in the Virtual Campus administrative building. The hallway "+
            "continues to the east of here. West is the building exit, to "+
            "the world outside.\n%^GREEN%^There is a large sign on the wall "+
            "you can read.%^RESET%^");
    SetExits( ([
                "west" : "/domains/campus/room/usquare",
                "east" : "/domains/campus/room/corridor3"
                ]) );
    SetItems( (["corridor" : "This is a carpeted corridor, leading west.",
                "floor" : "The carpet is purple, and somewhat institutional.",
                "sign":"A large sign on the wall. To read it, 'read sign'.",
                "carpet" : "The carpet is purple, and somewhat institutional."]) );
    SetRead("sign", (: readSign :) );
    SetProperty("no attack", 1);
}
int CanReceive(object ob) {
    if(ob && ob->GetRace() == "rodent"){
        message("info","You are repelled by rodenticide.",ob);
        return 0;
    }
    return ::CanReceive(ob);
}
void init(){
    ::init();
}
