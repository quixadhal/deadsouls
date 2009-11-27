#include <lib.h>
#include ROOMS_H
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
    if(DISABLE_INTERMUD) return 1;
    if(!OK){
        Retries++;
        update(INTERMUD_D);
    }
    else {
        if(Retries > 0 && INTERMUD_D->GetConnectedStatus()){
            tell_room(ROOM_ARCH,"The Arch Room loudspeaker announces: \"%^BOLD%^CYAN%^"
                    "Intermud connection is %^BOLD%^GREEN%^ONLINE%^BOLD%^CYAN%^.%^RESET%^\"");
            load_object(ROOM_ARCH)->SetImud(1);
        }

        Retries = 0;
    }
    if(Retries == 2 && !(INTERMUD_D->GetConnectedStatus())){
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
    mixed chans;
    if(DISABLE_INTERMUD) return 1;
    chans = INTERMUD_D->GetChannels();
    Pinging = 1;
    OK = 0;
    if(!sizeof(muds)) {
        Pinging = 0;
        return 0;
    }
    foreach(string mud in muds){
        INTERMUD_D->eventWrite(({ "auth-mud-req", 5, mud_name(), 0, mud, 0 }));
    }
    if(!sizeof(chans)){
        string rtr = INTERMUD_D->GetNameservers()[0][0];
        INTERMUD_D->eventWrite(({ "chanlist-req", 5, mud_name(), 0, rtr, 0 }));
    }
    return 1;
}

void create() {
    daemon::create();
    SetNoClean(1);
    if(!DISABLE_INTERMUD) set_heart_beat(1);
    else {
        set_heart_beat(0);
        Pinging = 0;
    }
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
    if(!DISABLE_INTERMUD){
        if(!(counter % (PING_INTERVAL + 2))) CheckOK();
        if(!(counter % (PING_INTERVAL))) eventPing();
    }
    if(counter > (PING_INTERVAL * 1000)) counter = 0;
}

int GetPinging(){
    return Pinging;
}

int SetOK(){
    if(DISABLE_INTERMUD) return 1;
    OK = 1;
    load_object(ROOM_ARCH)->SetImud(1);
}
