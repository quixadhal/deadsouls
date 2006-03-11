#include <lib.h>
#include <daemons.h>
#include <rooms.h>

inherit LIB_ROOM;

string LoadIP(){
    return INTERMUD_D->GetMudList()["Dead Souls"][1];
}

string LoadPort(){
    return INTERMUD_D->GetMudList()["Dead Souls"][2];
}

void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("the telnet room");
    SetLong("From this room, you can attempt "+
      "to connect to Dead Souls MUD. This is useful if you "+
      "need to ask questions but the intermud connection is "+
      "down. It is also a way to get a sneak peek at what's "+
      "coming in the next release of Dead Souls, because "+
      "Dead Souls MUD is where all new fixes and features "+
      "are tested before release."+
      "\n\tIf the connection "+
      "fails, email cratylus@users.sourceforge.com to ask for help. "+
      "Once you connect to Dead Souls, type \"dcon\" to "+
      "return to your own mud. \n\n"+
      "To connect type \"connect\" \n"+
      "The Creators' Hall is east  of here.");
    SetExits( ([
	"east" : "/domains/default/room/wiz_hall"
      ]) );
    SetObviousExits("e");
    SetNoModify(1);
}

void init(){
    object ding;
    ::init();
    if(!present("telnet_room_client",this_player())){
	ding=new("/secure/obj/tc");
	ding->eventMove(this_player());
	ding->SetConnection(LoadIP()+" "+LoadPort());
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
