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
#include <save.h>
#include <config.h>
#include <daemons.h>
#include <network.h>
#include "include/intermud.h"

inherit LIB_CLIENT;

private int Password;
private class list MudList, ChannelList;
private mapping Banned;
private mixed *Nameservers;
private static int Connected, Tries;

static void create() {
    client::create();
    Connected = 0;
    Password = 0;
    Tries = 0;
    Banned = ([]);
    Nameservers = ({ ({ "*gjs", "198.144.203.194 9000" }) });
    MudList = new(class list);
    ChannelList = new(class list);
    MudList->ID = -1;
    MudList->List = ([]);
    ChannelList->ID = -1;
    ChannelList->List = ([]);
    if( file_size( SAVE_INTERMUD __SAVE_EXTENSION__ ) > 0 )
	unguarded( (: restore_object, SAVE_INTERMUD, 1 :) );
    SetNoClean(1);
    SetDestructOnClose(1);
    SetSocketType(MUD);
    call_out( (: Setup :), 2);
}


static void Setup() {
    string ip;
    int port;

    if( !Nameservers || !sizeof(Nameservers) ) return;
    sscanf(Nameservers[0][1], "%s %d", ip, port);
    if( eventCreateSocket(ip, port) < 0 ) return;
    eventWrite( ({ "startup-req-3", 5, mud_name(), 0, Nameservers[0][0], 0,
	Password, MudList->ID, ChannelList->ID, query_host_port(),
	PORT_OOB, PORT_UDP, mudlib() + " " + mudlib_version(), 
	mudlib() + " " + mudlib_version(), version(), "LPMud",
	MUD_STATUS, ADMIN_EMAIL,
	(mapping)SERVICES_D->GetServices(), ([]) }) );
}

static void eventRead(mixed *packet) {
    string *cles;
    mixed val;
    string ns, cle;
    int i, maxi;

    if( !packet || sizeof(packet) < 6 ) return; /* should send error */
    if( Banned[packet[2]] ) {
	string reason = Banned[packet[2]];

	eventWrite(({ "error", 5, mud_name(), 0, packet[2],
	    packet[3], "unk-user", 
	    "Your mud is not allowed to send to Dead Souls.",
	    packet }));

	return;
    }
    switch(packet[0]) {
    case "startup-reply":
	if( sizeof(packet) != 8 ) return;  /* should send error */
	if( !sizeof(packet[6]) ) return;
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
	if( sizeof(packet) != 8 ) return;
	if( packet[6] == MudList->ID ) return; 
	if( packet[2] != Nameservers[0][0] ) return;
	MudList->ID = packet[6];
	foreach(cle, val in packet[7]) {
	    if( !val && MudList->List[cle] != 0 ) 
		map_delete(MudList->List, cle);
	    else if( val ) MudList->List[cle] = val;
	}
	save_object(SAVE_INTERMUD);
	return;
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
	//	  if( packet[6] == ChannelList->ID ) return; 
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
	break;
    case "error":
	SERVICES_D->eventReceiveError(packet);
	break;
    default:
	break;
    }
}

static void eventSocketClose() {
    int extra_wait;

    extra_wait = (Tries++) * 20;
    if( extra_wait > 600 ) extra_wait = 600;
    Connected = 0;
    call_out( (: Setup :), 20 + extra_wait);
}

static void eventConnectionFailure() {
    if( Connected ) return;
    error("Failed to find a useful name server.\n");
}

int SetDestructOnClose(int x) { return 0; }

int SetSocketType(int x) { return client::SetSocketType(MUD); }

string GetMudName(string mud) {
    string *lc, *uc;
    int x;

    if( MudList->List[mud] ) return mud;
    lc = map(uc = keys(MudList->List), function(string str) {
	  if( !str ) return "";
	  else return lower_case(str);
	});
      x = member_array(lower_case(mud), lc);
      if( x < 0 ) return 0;
      else return uc[x];
  }

    mapping GetMudList() { return copy(MudList->List); }

    string *GetMuds() { return keys(MudList->List); }

    mapping GetChannelList() { return copy(ChannelList->List); }

    string *GetChannels() { return keys(ChannelList->List); }

    string *GetMatch(string mud) {
	string *uc, *lc;

	mud = lower_case(mud);
	lc = map(uc = keys(MudList->List), (: lower_case :));
	return map(filter(regexp(lc, "^"+mud, 1), (: intp :)), (: $(uc)[$1] :));
    }

    string GetNameserver() { return Nameservers[0][0]; }

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

#endif /* __PACKAGE_SOCKETS__ */

