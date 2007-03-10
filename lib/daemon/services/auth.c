#define SERVICE_AUTH

#include <daemons.h>
#include <rooms.h>
#include <message_class.h>

void eventReceiveAuthReply(mixed array packet) {
    object pinger;
    object *pingers = filter( users(), (: $1->GetProperty("pinging") :) );
    //tc("mud: "+packet[2]+", token: "+packet[6],"blue");
    OOB_D->ReceivedToken(packet[2],packet[6]);
    PING_D->SetOK();
    if(pinger = find_object("/secure/daemon/ping")){
	if(pinger->GetPinging()){
	    pinger->SetOK(1);
	}
    }
    tn("Auth reply received from "+packet[2]+".","white");
    tn("Ping packet: "+identify(packet),"white");
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
    string header = "auth-mud-reply";
    int token = random_numbers(9,1);
    PING_D->SetOK();
    tn(packet[0]+" received from "+packet[2]+".","white");
    if(packet[0] == "ping-req") header = "ping-reply";
    INTERMUD_D->eventWrite( ({header, 5, mud_name(), 0, packet[2],
	0, token }) );
    //tc("mud: "+packet[2]+", token: "+token,"green");
    //tc("token: "+token,"green");
    OOB_D->SentToken(packet[2],token);
    //tc("mud: "+packet[2]+", token: "+token);
    if(file_exists("/tmp/muds.txt"))
	mudlist = read_file("/tmp/muds.txt");
    if((!grepp(mudlist,packet[2]) || packet[2] == "DeadSoulsNew" ||
	packet[2] == "DeadSoulsWin") && packet[2] != mud_name()){
	write_file("/tmp/muds.txt",packet[2]+"\n"); 
	//tc("We have a new mud! "+packet[2]+" has joined intermud.","red");
	tn("We have a new mud! "+packet[2]+" has joined intermud.","red");
    }
}
