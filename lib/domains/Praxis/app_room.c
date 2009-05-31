#include <lib.h>

inherit LIB_ROOM;

void create() {
    ::create();
    SetProperties( ([
                "light":2,
                "indoors":1
                ]) );
    SetShort("approval room");
    SetLong("This is the meeting room of the approval team. In this room "
            "problems, suggestions and new approval changes can be posted on the "
            "board so that everyone is informed about the newest stuff. Up from this "
            "room is the hall of immortals.");
    SetExits( ([
                "up": __DIR__ "adv_inner"
                ]) );
}
void reset() {
    ::reset();
    //if(!present("approval board")) 
    //new( __DIR__ "obj/misc/app_board")->move(this_object());
}
void init(){
    ::init();
}
