#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("the telnet room");
    SetLong("From this room, you can attempt "+
      "to connect to Frontiers MUD. This is useful to you because "+
      "Dead Souls lib documentation is only available on Frontiers, "+
      "for copyright reasons beyond our control. If the connection "+
      "fails, email cratylus@users.sourceforge.com to ask for help. "+
      "On Frontiers, visit the Campus Bookstore and buy a Creator's "+
      "Manual from Kim. This contains all the documentation you "+
      "need to build a great MUD. \n"+
      "Once you connect to Frontiers, type \"dcon\" to "+
      "return to your own mud. \n\n"+
      "To connect type \"connect\" \n"+
      "The default start room is south of here.");
    SetExits( ([
	"south" : "/domains/default/room/start"
      ]) );
    SetObviousExits("s");
}

void init(){
    object ding;
    ::init();
    if(!present("telnet_room_client",this_player())){
	ding=new("/secure/obj/tc");
	ding->eventMove(this_player());
	ding->force_connect(this_player());
    }
}

int CanRelease(object ob){
    if(present("telnet_room_client",ob)){
	present("telnet_room_client",ob)->eventDestruct();
    }
    return 1;
}

int CanReceive(object ob) {
    if(interactive(ob)){
	tell_object(this_object(),ob->GetName()+" is about to enter the telnet room.");
    }
    return 1;
}
