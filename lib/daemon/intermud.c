/*    /daemon/intermud.c
 *    from the Dead Souls LPC Library
 *    daemon handling the InterMUD-3 Protocol
 *    created by Descartes of Borg 950506
 *    Version: @(#) intermud.c 1.7@(#)
 *    Last modified: 96/12/14
 */

#ifndef __PACKAGE_SOCKETS__
#error You should not try and load /daemon/intermud.c with no sockets package.
#else

#include <lib.h>
#include <privs.h>
#include <save.h>
#include <config.h>
#include <daemons.h>
#include <network.h>
#include <rooms.h>
#include "include/intermud.h"

inherit LIB_CLIENT;

private int Password;
private class list MudList, ChannelList;
private mapping Banned;
private mixed *Nameservers;
private static int Connected, Tries;
private static int SocketStat = -1;

mapping ExtraInfo();

static void create() {
    client::create();
    tn("INTERMUD_D: prev: "+identify(previous_object(-1)),"red");
    Connected = 0;
    Password = 0;
    Tries = 0;
    Banned = ([]);
    MudList = new(class list);
    ChannelList = new(class list);
    MudList->ID = -1;
    MudList->List = ([]);
    ChannelList->ID = -1;
    ChannelList->List = ([]);
    if( file_size( SAVE_INTERMUD __SAVE_EXTENSION__ ) > 0 )
	unguarded( (: restore_object, SAVE_INTERMUD, 1 :) );
    Nameservers = ({ ({ "*i4", "204.209.44.3 8080" }) });
    SetNoClean(1);
    tn("INTERMUD_D reloaded.");
    tn("Loading object stack: "+get_stack(),"red");
    tn("Loading object trail: "+identify(previous_object(-1)),"red");
    SetDestructOnClose(1);
    SetSocketType(MUD);
    if(DISABLE_INTERMUD == 1){
	call_out( (: eventDestruct :), 2);
    }
    else call_out( (: Setup :), 2);
}

void FirstPing(){
    PING_D->eventPing();
}

static void Setup() {
    string ip;
    int port;

    if( !Nameservers || !sizeof(Nameservers) ) return;
    sscanf(Nameservers[0][1], "%s %d", ip, port);
    SocketStat = eventCreateSocket(ip, port);
    if( SocketStat < 0 ) return;
    tn("INTERMUD_D: SocketStat: "+SocketStat);
    eventWrite( ({ "startup-req-3", 5, mud_name(), 0, Nameservers[0][0], 0,
	Password, MudList->ID, ChannelList->ID, query_host_port(),
	PORT_OOB, PORT_UDP, mudlib() + " " + mudlib_version(), 
	mudlib() + " " + mudlib_version(), version(), "LPMud",
	MUD_STATUS, ADMIN_EMAIL,
	(mapping)SERVICES_D->GetServices(), ExtraInfo() }) );
    tn("INTERMUD_D setup: "+identify( ({
	  "startup-req-3", 5, mud_name(), 0, Nameservers[0][0], 0,
	  Password, MudList->ID, ChannelList->ID, query_host_port(),
	  PORT_OOB, PORT_UDP, mudlib() + " " + mudlib_version(),
	  mudlib() + " " + mudlib_version(), version(), "LPMud",
	  MUD_STATUS, ADMIN_EMAIL,
	  (mapping)SERVICES_D->GetServices(), ([]) }) ), "red");;
    call_out( (: FirstPing :), 5);
}

int GetConnectedStatus(){
    if(SocketStat > -1 && socket_status(SocketStat)[1] == "DATA_XFER") return 1;
    else return 0;
}

void eventClearVars(){
    if( !((int)master()->valid_apply(({ PRIV_ASSIST, INTERMUD_D }))) )
	error("Illegal attempt to reset intermud: "+get_stack()+" "+identify(previous_object(-1)));
    Connected = 0;
    Tries = 0;
    MudList = new(class list);
    ChannelList = new(class list);
    MudList->ID = -1;
    MudList->List = ([]);
    ChannelList->ID = -1;
    ChannelList->List = ([]);
    save_object(SAVE_INTERMUD);
}

static void eventRead(mixed *packet) {
    mixed val;
    string cle;

    if( !packet || sizeof(packet) < 6 ) return; /* should send error */
    if( Banned[packet[2]] ) {

	eventWrite(({ "error", 5, mud_name(), 0, packet[2],
	    packet[3], "unk-user", 
	    "Your mud is not allowed to send to Dead Souls.",
	    packet }));

	return;
    }
    switch(packet[0]) {
    case "startup-reply":
	log_file("intermud",identify(packet));
	tn("INTERMUD_D: "+identify(packet),"red");
	if( sizeof(packet) != 8 ) {
	    //tn("We don't like the mudlist packet size.","red");
	    return;  
	}
	if( !sizeof(packet[6]) ) {
	    //tn("We don't like an absence of packet element 6.","red");
	    return;
	}
	/* Start of Tricky's patch */
	if( packet[2] != Nameservers[0][0] ) {
	    tn("Illegal startup-reply from mud " + packet[2], "red");
	    return;
	}
	/* End of Tricky's patch */ 
	if( packet[6][0][0] == Nameservers[0][0] ) {
	    Nameservers = packet[6];
	    Connected = Nameservers[0][0];
	    Password = packet[7];
	    save_object(SAVE_INTERMUD);
	}
	else {
	    Nameservers = packet[6];
	    Setup();
	}
	return;
    case "mudlist":
	tn("INTERMUD_D mudlist received.","red");
	log_file("mudlist_packet",identify(packet),1);
	if( sizeof(packet) != 8 ) {
	    //tn("We don't like the mudlist packet size.","red");
	    return;  
	}
	if( packet[6] == MudList->ID )  {
	    //tn("We don't like packet element 6. It is: "+identify(packet[6]),"red");
	    //tn("We will continue anyway.","red");
	}
	if( packet[2] != Nameservers[0][0] ) {
	    //tn("We don't like packet element 2. It is: "+identify(packet[2]),"red");
	    return;
	}

	MudList->ID = packet[6];
	foreach(cle, val in packet[7]) {
	    string tmp = "";
	    if(cle){
		string lib = "unknown";
		if(val && sizeof(val) > 5 && arrayp(val)) lib = val[5];
		tmp += "%^BOLD%^CYAN%^Processing mud: "+identify(cle)+
		", lib: "+lib;
		if(val){
		    if(!val[0]) tmp += " %^RED%^BOLD%^offline%^RESET%^";
		    else tmp += " %^GREEN%^BOLD%^ONLINE%^RESET%^";
		}
		else tmp += " Status unknown";
		tn(tmp);
		tn(tmp,"cyan",ROOM_ARCH);
	    }
	    if( !val && MudList->List[cle] != 0 ) 
		map_delete(MudList->List, cle);
	    else if( val ) MudList->List[cle] = val;
	}
	save_object(SAVE_INTERMUD);
	return;
    case "ping-req":
	SERVICES_D->eventReceiveAuthRequest(packet);
	break;
    case "ping-reply":
	SERVICES_D->eventReceiveAuthReply(packet);
	break;
    case "auth-mud-req":
	SERVICES_D->eventReceiveAuthRequest(packet);
	break;
    case "auth-mud-reply":
	SERVICES_D->eventReceiveAuthReply(packet);
	break;
    case "channel-t":
	SERVICES_D->eventReceiveChannelTargettedEmote(packet);
	break;
    case "channel-e":
	SERVICES_D->eventReceiveChannelEmote(packet);
	break;
    case "channel-m":
	SERVICES_D->eventReceiveChannelMessage(packet);
	break;
    case "chan-who-reply":
	SERVICES_D->eventReceiveChannelWhoReply(packet);
	break;
    case "chan-who-req":
	SERVICES_D->eventReceiveChannelWhoRequest(packet);
	break;
    case "chan-user-req":
	SERVICES_D->eventReceiveChannelUserRequest(packet);
	break;
    case "chanlist-reply":
	tn("chanlist reply: "+identify(packet), "blue");
	if( packet[2] != Nameservers[0][0] ) return;
	ChannelList->ID = packet[6];
	foreach(cle, val in packet[7]) { 
	    if( !val && ChannelList->List != 0 ) 
		map_delete(ChannelList->List, cle);
	    else if( val ) ChannelList->List[cle] = val;
	} 
	save_object(SAVE_INTERMUD);
	SERVICES_D->eventRegisterChannels(packet[7]);
	return;
    case "emoteto":
	SERVICES_D->eventReceiveEmote(packet);
	break;
    case "finger-req":
	SERVICES_D->eventReceiveFingerRequest(packet);
	break;
    case "finger-reply":
	SERVICES_D->eventReceiveFingerReply(packet);
	break;
    case "locate-req":
	SERVICES_D->eventReceiveLocateRequest(packet);
	break;
    case "locate-reply":
	SERVICES_D->eventReceiveLocateReply(packet);
	break;
    case "tell":
	SERVICES_D->eventReceiveTell(packet);
	break;
    case "chan-user-reply":
	tn("INTERMUD_D: chan-user-reply received.","red");
    case "ucache-update":
	SERVICES_D->eventReceiveUcacheUpdate(packet);
	break;
    case "who-req":
	SERVICES_D->eventReceiveWhoRequest(packet);
	break;
    case "who-reply":
	SERVICES_D->eventReceiveWhoReply(packet);
	break;
    case "news":
	SERVICES_D->eventReceiveNews(packet);
	break;
    case "mail":
	SERVICES_D->eventReceiveMail(packet);
	break;
    case "mail-ok":
	SERVICES_D->eventReceiveMailOk(packet);
	break;
    case "file":
	tn("INTERMUD_D: file packet received.","red");
	break;
    case "error":
	SERVICES_D->eventReceiveError(packet);
	break;
    default:
	break;
    }
}

static void eventSocketClose() {
    //int extra_wait;

    //This appears to be malfunctioning.
    //
    //extra_wait = (Tries++) * 20;
    //if( extra_wait > 600 ) extra_wait = 600;
    //Connected = 0;
    //call_out( (: Setup :), 20 + extra_wait);
}

static void eventConnectionFailure() {
    if( Connected ) return;
    tn("INTERMUD_D: CONNECTION FAILED","red");
    error("Failed to find a useful name server.\n");
}

int SetDestructOnClose(int x) { return 0; }

static void eventClose(mixed arg){
    SocketStat = -1;
    tn("INTERMUD_D: socket closing!");
    ::eventClose(arg);
}

int SetSocketType(int x) { return client::SetSocketType(MUD); }

string GetMudName(string mud) {
    string *lc, *uc;
    mapping mudses = GetMudList();
    int x;

    if( MudList->List[mud] ) return mud;
    lc = map(uc = keys(MudList->List), function(string str) {
	  if( !str ) return "";
	  else return lower_case(str);
	});
      x = member_array(lower_case(mud), lc);
      if( x < 0 ){
	  foreach(string name in GetMuds()){
	      //if(mudses[name][1] == mud) return name;
	      if(mudses[name][1] +" "+mudses[name][2] == mud) return name;
	  }
	  return 0;
      }
      else return uc[x];
  }

    mapping GetMudList() { return copy(MudList->List); }

    string *GetMuds() { return keys(MudList->List); }

    string *GetLCMuds() {
	string *orig_arr, *new_arr;
	orig_arr = GetMuds();
	new_arr = ({});
	foreach(string namen in orig_arr){
	    new_arr += ({ lower_case(namen) });
	}
	return new_arr;
    }

    mapping GetChannelList() { return copy(ChannelList->List); }

    string *GetChannels() { return keys(ChannelList->List); }

    string *GetMatch(string mud) {
	string *uc, *lc;

	mud = lower_case(mud);
	lc = map(uc = keys(MudList->List), (: lower_case :));
	return map(filter(regexp(lc, "^"+mud, 1), (: intp :)), (: $(uc)[$1] :));
    }

    string GetNameserver() { return Nameservers[0][0]; }

    mixed *GetNameservers() { return copy(Nameservers); }

    int AddBanned(string mud, string reason) {
	if( !master()->valid_apply(({})) ) {
	    return 0;
	}
	if( !(mud = GetMudName(mud)) ) {
	    return 0;
	}
	Banned[mud] = reason;
	save_object(SAVE_INTERMUD);
	return 1;
    }

    int RawSend(string *packet){
	if(!this_player() || !archp(this_player())) return 0;
	eventWrite(packet);
	return 1;
    }

    string nextboot() {
	string str;
	int x, offset;

	if(DISABLE_REBOOTS){
	    return "never";
	}

	offset = (int)TIME_D->GetOffset(local_time()[9]);
	offset += EXTRA_TIME_OFFSET;

	x = (int)EVENTS_D->GetRebootInterval() * 3600;
	x = (time() - uptime()) + x;
	if(!LOCAL_TIME) 
	    x += offset * 3600;
	str = query_tz()+ " " + ctime(x);
	return str;
    }

    //This new packet element data added to be able
    //to handle liveupgrade stuff more sensibly.
    mapping ExtraInfo(){
	return ([
	  "native version" : native_version(),
	  "os build" : query_os_type(),
	  "upsince" : ctime(time() - uptime()),
	  "next boot" : nextboot(),
	  "ip" : HOST_IP,
	  "oob port" : PORT_OOB,
	]);
    }


#endif /* __PACKAGE_SOCKETS__ */
