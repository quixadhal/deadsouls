#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int Pinging = 0;
int OK = 0;
string *muds = ({ "Frontiers", "Dead Souls" });

int CheckOK(){
    Pinging = 0;
    if(!OK){
	//tc("not ok");
	update("/daemon/intermud");
    }
    else {
	//tc("ok");
    }
    return 1;
}

int eventPing(){
    Pinging = 1;
    OK = 0;
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
}
