#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("the telnet room");
    SetLong("From this room, you can attempt "+
      "to connect to Frontiers MUD. This was once useful to you because "+
      "Dead Souls lib documentation was only available on Frontiers, "+
      "for copyright reasons beyond our control. These days, the docs "+
      "are available to you in the creator's manual in the chest in "+
      "your workroom. You're still welcome to visit Frontiers, though. "+
      "\n\tIf the connection "+
      "fails, email cratylus@users.sourceforge.com to ask for help. "+
      "Once you connect to Frontiers, type \"dcon\" to "+
      "return to your own mud. \n\n"+
      "To connect type \"connect\" \n"+
      "The Creators' Hall is east  of here.");
    SetExits( ([
	"east" : "/domains/default/room/wiz_hall"
      ]) );
    SetObviousExits("e");
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
