#define SERVICE_AUTH

#include <daemons.h>
#include ROOMS_H
#include <message_class.h>

void eventReceiveAuthReply(mixed array packet) {
    object pinger;
    object *pingers = filter( users(), (: $1->GetProperty("pinging") :) );
    OOB_D->ReceivedToken(packet[2],packet[6]);
    PING_D->SetOK();
    if(pinger = find_object("/secure/daemon/ping")){
        if(pinger->GetPinging()){
            pinger->SetOK(1);
        }
    }
    tn("Auth reply received from "+packet[2]+".","white");
    //tn("Ping packet: "+identify(packet),"white");
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
    OOB_D->SentToken(packet[2],token);
    if(file_exists("/tmp/muds.txt"))
        mudlist = read_file("/tmp/muds.txt");
    if((!grepp(mudlist,packet[2]) || packet[2] == "DeadSoulsNew" ||
                packet[2] == "DeadSoulsWin") && packet[2] != mud_name()){
        write_file("/tmp/muds.txt",packet[2]+"\n"); 
        tn("We have a new mud! "+packet[2]+" has joined intermud.","red");
    }
}

void eventSendAuthRequest(string target){
    INTERMUD_D->eventWrite(({ "auth-mud-req", 5, mud_name(), 0, target, 0 }));
}
