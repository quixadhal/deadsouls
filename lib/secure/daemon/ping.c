#include <lib.h>
#include <config.h>
#include <rooms.h>
#include <daemons.h>

inherit LIB_DAEMON;

int Pinging = 0;
int OK = 0;
int Retries = 0;

string *muds = PINGING_MUDS + ({ mud_name() });

int CheckOK(){
    Pinging = 0;
    if(!OK){
	Retries++;
	update("/daemon/intermud");
    }
    else {
	if(Retries > 0){
	    tell_room(ROOM_ARCH,"The Arch Room loudspeaker announces: \"%^BOLD%^CYAN%^"
	      "Intermud connection is %^BOLD%^GREEN%^ONLINE%^BOLD%^CYAN%^.%^RESET%^\"");
	    load_object(ROOM_ARCH)->SetImud(1);
	}

	Retries = 0;
    }
    if(Retries == 2){
	tell_room(ROOM_ARCH,"The Arch Room loudspeaker announces: \"%^BOLD%^CYAN%^"
	  "Intermud connection is %^BOLD%^RED%^OFFLINE%^BOLD%^CYAN%^.%^RESET%^\"");
	rm("/tmp/muds.txt");
	load_object(ROOM_ARCH)->SetImud(0);
    }
    return 1;
}

int eventPing(){
    Pinging = 1;
    OK = 0;

    if(!sizeof(muds)) {
	Pinging = 0;
	return 0;
    }

    foreach(string mud in muds){
	INTERMUD_D->eventWrite(({ "auth-mud-req", 5, mud_name(), 0, mud, 0 }));
    }
    call_out( (: CheckOK :), 10);
    call_out( (: eventPing :), 900);
    return 1;
}

void create() {
    daemon::create();
    SetNoClean(1);
    call_out( (: eventPing :), 20);
}

int GetPinging(){
    return Pinging;
}

int SetOK(){
    OK = 1;
    load_object(ROOM_ARCH)->SetImud(1);
}
