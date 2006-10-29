#include <lib.h>
#include <config.h>
#include <rooms.h>
#include <daemons.h>

inherit LIB_DAEMON;

int Pinging = 0;
int OK = 0;
int Retries = 0;
int counter = 0;
int last_time = time();

string *muds = PINGING_MUDS + ({ mud_name() });

int CheckOK(){
    string list = load_object("/cmds/players/mudlist")->cmd("");
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
    write_file("/www/mudlist.txt",timestamp()+"\n",1);
    write_file("/www/mudlist.txt",""+list);
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
    return 1;
}

void create() {
    daemon::create();
    SetNoClean(1);
    set_heart_beat(1);
}

void DeadMan(){
    //    This breaks things.
    //    int tmptime = time();
    //    if(last_time + 60 < tmptime){
    //	object *clones = filter(objects(), (: inherits(LIB_ROOM,$1) :));
    //	clones->eventDestruct();
    //
    //	clones = filter(objects(), (: clonep($1) :));
    //	clones->eventDestruct();
    //    }
    //    else last_time = tmptime;
}

void heart_beat(){
    counter++;
    DeadMan();
    if(!(counter % 30)) CheckOK();
    if(!(counter % 300)) eventPing();
    if(counter > 10000) counter = 0;
}

int GetPinging(){
    return Pinging;
}

int SetOK(){
    OK = 1;
    load_object(ROOM_ARCH)->SetImud(1);
}
