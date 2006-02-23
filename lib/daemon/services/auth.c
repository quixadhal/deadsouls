#define SERVICE_AUTH

#include <daemons.h>
#include <rooms.h>
#include <message_class.h>

void eventReceiveAuthReply(mixed array packet) {
    object pinger;
    //tc("eventReceiveAuthReply: "+identify(packet),"cyan");
    if(pinger = find_object("/secure/daemon/ping")){
	if(pinger->GetPinging()){
	    pinger->SetOK(1);
	}
    }
}

void eventReceiveAuthRequest(mixed array packet) {
    //tc("eventReceiveAuthRequest: "+identify(packet),"green");
    INTERMUD_D->eventWrite( ({"auth-mud-reply", 5, mud_name(), 0, packet[2],
	0, (random(9999) * 10000) + 1138  }) );
}


