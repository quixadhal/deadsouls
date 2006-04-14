#define SERVICE_AUTH

#include <daemons.h>
#include <rooms.h>
#include <message_class.h>

void eventReceiveAuthReply(mixed array packet) {
    object pinger;
    object *pingers = filter( users(), (: $1->GetProperty("pinging") :) );
    PING_D->SetOK();
    if(pinger = find_object("/secure/daemon/ping")){
	if(pinger->GetPinging()){
	    pinger->SetOK(1);
	}
    }
    tn("Auth reply received from "+packet[2]+".","white");
    if(sizeof(pingers)){
	foreach(object dude in pingers){
	    tell_player(dude, packet[2]+" has just replied to a ping request from "+
	      packet[4]+".");
	    dude->SetProperty("pinging",0);
	}
    }
}

void eventReceiveAuthRequest(mixed array packet) {
    string mudlist = "";
    PING_D->SetOK();
    tn("Auth request received from "+packet[2]+".","white");
    INTERMUD_D->eventWrite( ({"auth-mud-reply", 5, mud_name(), 0, packet[2],
	0, (random(9999) * 10000) + 1138  }) );
    if(file_exists("/tmp/muds.txt"))
	mudlist = read_file("/tmp/muds.txt");
    if(!grepp(mudlist,packet[2]) || packet[2] == "DeadSoulsNew" ||
      packet[2] == "DeadSoulsWin"){
	write_file("/tmp/muds.txt",packet[2]+"\n"); 
	//tc("We have a new mud! "+packet[2]+" has joined intermud.","red");
	tn("We have a new mud! "+packet[2]+" has joined intermud.","red");
    }

}


