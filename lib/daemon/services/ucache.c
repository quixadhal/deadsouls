/*    /daemon/services/ucache.c
 *    From the Dead Souls V Object Library
 *    Intermud 3 ucache service implementation
 *    created by Tim 961211
 *    Version: @(#) ucache.c 1.2@(#)
 *    Last modified: 96/12/14
 */

#define SERVICE_UCACHE

// Number of days until an item is removed from the cache
#define UCACHE_INFO_LIFETIME 4

class uinfo {
    string Visname;
    int    Gender;
    int    TimeStamp;
}

private mapping Ucache = ([ ]);

void eventReceiveUcacheUpdate(mixed array packet) {
    string mud = packet[2], user = packet[6];
    class uinfo ui;

    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( !Ucache[mud] ) Ucache[mud] = ([ ]);
    if( !(ui = Ucache[mud][user]) ) Ucache[mud][user] = ui = new(class uinfo);
    ui->Visname   = (string)packet[7];
    ui->Gender    = (int)packet[8];
    ui->TimeStamp = time();
}

static void eventSendChannelUserRequest(string who, string where) {
    string pl;

    INTERMUD_D->eventWrite( ({ "chan-user-req", 5, mud_name(), 0, where, 0, 
			       who }) );
}

void eventClearUcache() {
    if( this_player(1) && !archp(this_player(1)) ) return;
    Ucache = ([ ]);
}

void eventDisplayUcache(object agent) {
    string array lines;

    if( !agent || (this_player(1) && !archp(this_player(1))) ) return;
    lines = ({ "Mud        User       VisName    Gender" });
    foreach(string mud, mapping uc in Ucache) {
	foreach(string user, class uinfo ui in uc)
	    lines += ({ sprintf("%-:10s %-:10s %-:10s %d",
				mud, user, ui->Visname, ui->Gender) });
    }
    agent->eventPage(lines);
}

static void eventCompactUcache() {
    int ts = time() - (UCACHE_INFO_LIFETIME * 3600 * 24);

    foreach(string mud, mapping uc in Ucache) {
	foreach(string user, class uinfo ui in uc)
	        if( ui->TimeStamp < ts ) map_delete(uc, user);
    }
    eventSave();
    call_out((: eventCompactUcache :), 3600);
}

string GetRemoteDisplayName(string user, string mud) {
    class uinfo ui;

    if( !Ucache[mud] || !(ui = Ucache[mud][user]) ) {
	eventSendChannelUserRequest(user, mud);
	return 0;
    }
    else {
	ui->TimeStamp = time();
	return ui->Visname;
    }
}

string GetRemoteGender(string user, string mud) {
    class uinfo ui;

    if( !Ucache[mud] || !(ui = Ucache[mud][user]) ) {
	eventSendChannelUserRequest(user, mud);
	return 0;
    }
    else {
	ui->TimeStamp = time();
	switch( ui->Gender ) {
	case 0:  return "male";
	case 1:  return "female";
	case 2:  return "neuter";
	default: return 0;
	}
    }
}
