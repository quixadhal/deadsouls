/*    /secure/daemon/character.c
 *    from the Dead Souls V Object Library
 *    the multi-character management daemon
 *    created by Descartes of Borg 951216
 */

#include <lib.h>
#include <save.h>
#include <privs.h>
#include <link.h>
#include <config.h>
#include "include/character.h"

inherit LIB_DAEMON;

private mapping Links;

static void create() {
    daemon::create();
    SetNoClean(1);
    Links = ([]);
    if( unguarded( (: file_size(SAVE_CHARACTER __SAVE_EXTENSION__) :)) > 0 )
      unguarded((: restore_object, SAVE_CHARACTER :));
}

mixed eventConnect(string who) {
    class char_link c;
    string prime;
    int min_wait;
    
    if( base_name(previous_object()) != LIB_CONNECT ) return 0;
    if( !(c = Links[prime = who]) ) {
	foreach(string pl, class char_link cl in Links) {
	    if( member_array(who, cl->Secondaries) != -1 ) {
		prime = pl;
		c = cl;
		break;
	    }
	}
	if( !c ) return 1;
    }
    foreach(string ind in c->Secondaries + ({ prime })) {
	if( find_player(ind) )  {
	    if( who == ind ) return 1;
	    if(member_group(who, PRIV_SECURE) || member_group(who,PRIV_ASSIST))
	      return 1;
	    else if( archp(find_player(ind)) ) return 1;
	    else return "You have a linked character currently logged in.\n";
	}
    }
    if( LINK_WAIT_PERIOD > uptime() ) min_wait = uptime();
    else min_wait = LINK_WAIT_PERIOD;
    if( c->LastOnDate > time() - min_wait ) {
	string tmp;
	int x;

	if( c->LastOnWith == who ) return 1;
	x = c->LastOnDate + min_wait - time();
	if( x < 60 ) tmp = consolidate(x, "a second");
	else tmp = consolidate(x/60, "a minute");
	if( !(member_group(who, PRIV_SECURE) || member_group(who,PRIV_ASSIST)))
	  return "\nYour character " + capitalize(c->LastOnWith) +
	    " recently logged in at " + ctime(c->LastOnDate) + ".\n" +
	    "You must wait another " + tmp + ".\n";
    }
    c->LastOnDate = time();
    c->LastOnWith = who;
    save_object(SAVE_CHARACTER);
    return 1;
}
	
mixed eventLink(string primary, string secondary, string email) {
    class char_link ch;

    if( !((int)master()->valid_apply(({ PRIV_LAW }))) )
      return "Permission denied.";
    if( !user_exists(primary = convert_name(primary)) )
      return "No such user: primary";
    if( !email ) return "Email is required for linking.";
    secondary = convert_name(secondary);
    if( Links[primary] ) {
	ch = Links[primary];
	if( Links[secondary] ) {
	    class char_link c2;

	    c2 = Links[secondary];
	    ch->Secondaries += ({ secondary });
	    ch->Secondaries += c2->Secondaries;
	    ch->Secondaries = distinct_array(ch->Secondaries);
	    ch->Email = email;
	    if( ch->LastOnDate < c2->LastOnDate ) {
		ch->LastOnDate = c2->LastOnDate;
		ch->LastOnWith = c2->LastOnWith;
	    }
	    Links[primary] = ch;
	    map_delete(Links, secondary);
	    if( !save_object(SAVE_CHARACTER) ) return "Error in saving.";
	    return 1;
	}
    }
    else if( Links[secondary] ) {
	ch = Links[secondary];
	map_delete(Links, secondary);
    }
    else ch = new(class char_link);
    if( ch->Secondaries ) ch->Secondaries += ({ secondary });
    else ch->Secondaries = ({ secondary });
    ch->Email = email;
    ch->LastOnDate = 0;
    ch->LastOnWith = primary;
    Links[primary] = ch;
    if( !save_object(SAVE_CHARACTER) ) return "Error in saving.";
    return 1;
}

mixed eventSaveTime() {
    class char_link c;
    object ob;
    string who;
    
    if( !userp(ob = previous_object()) ) return 0;
    who = (string)ob->GetKeyName();
    if( !(c = Links[who]) ) {
	foreach(string pl, class char_link cl in Links) {
	    if( member_array(who, cl->Secondaries) != -1 ) {
		c = cl;
		break;
	    }
	}
	if( !c ) return 1;
    }
    c->LastOnDate = time();
    c->LastOnWith = who;
    unguarded((: save_object, SAVE_CHARACTER :));
    return 1;
}

mixed eventUnlink(string primary, string who) {
    class char_link ch;
    
    if( !((int)master()->valid_apply(({ PRIV_ASSIST }))) )
      return "Permission denied.";
    primary = convert_name(primary);
    who = convert_name(who);
    if( !(ch = Links[primary]) ) return "No such primary character.";
    if( who == primary ) {
	if( sizeof(ch->Secondaries) < 2) {
	    map_delete(Links, primary);
	    save_object(SAVE_CHARACTER);
	    return 1;
	}
	primary = ch->Secondaries[0];
	ch->Secondaries = ch->Secondaries[1..];
	map_delete(Links, who);
	Links[primary] = ch;
	save_object(SAVE_CHARACTER);
	return 1;
    }
    if( member_array(who, ch->Secondaries) == -1 )
      return "Invalid secondary character for " + primary + ".";
    ch->Secondaries -= ({ who });
    Links[primary] = ch;
    save_object(SAVE_CHARACTER);
    return 1;
}

mapping GetLinks() {
    if( !((int)master()->valid_apply(({ PRIV_ASSIST }))) ) return ([]);
    else return Links;
}

mapping GetLink(string who) {
    class char_link ch;

    who = convert_name(who);
    if( Links[who] ) ch = Links[who];
    else {
	class char_link l;
	string pl;
	
	foreach(pl, l in Links) {
	    if( member_array(who, l->Secondaries) != -1 ) {
		ch = l;
		who = pl;
		break;
	    }
	}
	if( !ch ) return 0;
    }
    return ([ "primary" : who, "last char" : ch->LastOnWith,
	   "secondaries" : ch->Secondaries, "last on" : ch->LastOnDate ]);
}
