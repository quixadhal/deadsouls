/*    /daemon/chat.c
 *    from the Dead Souls 3.2 Mudlib
 *    daemon to handle all mud chat channels
 *    created by Descartes of Borg 931220
 */

#include <lib.h>
#include <pov.h>
#include <daemons.h>
#include <origin.h>
#include <message_class.h>
#include "include/chat.h"

inherit LIB_DAEMON;

static private mapping Channels;

static void create() {
    object pl;

    daemon::create();
    SetNoClean(1);
    Channels = ([]);
    foreach(pl in users()) {
	string *chans;
	string channel;

	if( !(chans = (string *)pl->GetChannels()) ) continue;
	foreach(channel in chans) {
            if( !Channels[channel] ) Channels[channel] = ({});
            Channels[channel] = distinct_array(Channels[channel] + ({ pl }));
        }
    }
}

string *eventRegisterMember(string *chans) {
    string *tmp;
    object ob;
    string channel;

    if( !living(ob = previous_object()) ) return ({});
    tmp = ({});
    foreach(channel in chans) {
	/* just check out for secure channels */
	switch(channel) {
            case "admin":
	      if( !archp(ob) ) break;
            case "cre": case "intercre": case "gossip": case "intergossip":
	      if( !creatorp(ob) ) break;
            default:
	      if( !Channels[channel]) Channels[channel] = ({});
	      Channels[channel] = distinct_array(Channels[channel] + ({ ob }));
	      tmp += ({ channel });
	}
    }
    return tmp;
}

string *eventRemoveMember(string *chans) {
    object ob;
    string channel;

    if( !living(ob = previous_object()) ) return({});
    foreach(channel in chans) {
        if( !Channels[channel] ) continue;
        else Channels[channel] -= ({ ob });
        if( !sizeof(Channels[channel]) ) map_delete(Channels, channel);
    }
    return chans;
}

int cmdChannel(string verb, string str) {
    string msg, name, rc, target, targetkey, target_msg;
    object ob = 0;
    int emote;

    if( verb == "list" ) {
	string *who;
	string ch, mud;

	if( !str ) return 0;
	if( sscanf(str, "%s@%s", ch, mud) == 2 ) {
	    if( !Channels[ch] ) return 0;
	    if( member_array(this_player(), Channels[ch]) == -1 ) return 0;
	    if( ch == (ch = GetRemoteChannel(ch)) ) return 0;
	    if( !(mud = (string)INTERMUD_D->GetMudName(mud)) ) {
		this_player()->eventPrint(mud_name() + " is not aware of " 
					  "such a place.", MSG_ERROR);
		return 1;
	    }
	    SERVICES_D->eventSendChannelWhoRequest(ch, mud);
	    this_player()->eventPrint("Remote listing request sent.",
				      MSG_SYSTEM);
	    return 1;
	}
        else ch = str;
	if( !Channels[ch] ) return 0;
	if( member_array(this_player(), Channels[str]) == -1 ) return 0;
	who = GetChannelList(str);
	msg = "Online: " + implode(who, "   ");
	this_player()->eventPrint(msg, MSG_SYSTEM);
	return 1;
    }
    if( !Channels[verb] ) {
        if( sscanf(verb, "%semote", verb) ) {
	    string emote_cmd, remains;
	    mixed array msg_data;
	    int i;
	    
	    if( !Channels[verb] ) {
		return 0;
	    }
	    rc = GetRemoteChannel(verb);
	    sscanf(str, "%s %s", emote_cmd, remains);
	    if( !emote_cmd ) {
		emote_cmd = str;
		remains = 0;
	    }
	    if( !remains ) {
		msg_data = SOUL_D->GetChannelEmote(emote_cmd, "");
		if( !msg_data ) {
		    str = "$N " + str;
		}
	    }
	    else {
		if( ob = find_living(target = convert_name(remains)) ) {
		    msg_data = SOUL_D->GetChannelEmote(emote_cmd, "LIV");
		    if( !msg_data ) {
			str = "$N " + str;
			target = 0;
		    }
		}
		else if( strsrch(target, "@") == -1 || rc == verb ) {
		    string array words = explode(remains, " ");

		    target = "";
		    for(i=0; i<sizeof(words); i++) {
			target += lower_case(words[i]);
			if( ob = find_living(target) ) {
			    if( i < sizeof(words)-1 ) {
				remains = implode(words[(i+1)..], " ");
			    }
			    else {
				remains = 0;
			    }
			    msg_data = SOUL_D->GetChannelEmote(emote_cmd,
							       "LIV STR",
							       remains);
			    break;
			}
		    }
		    if( !ob ) {
			msg_data = SOUL_D->GetChannelEmote(emote_cmd, "STR",
							   remains);
			target = 0;
		    }
		    if( !msg_data ) {
			str = "$N " + str;
			target = 0;
		    }
		}
		else if( rc != verb ) {
		    string array words;

		    i = strsrch(remains, "@", -1);
		    if( i >= strlen(remains)-1 ) {
			msg_data = SOUL_D->GetChannelEmote(emote_cmd, "STR",
							   remains);
			target = 0;
		    }
		    else {
			string mud;
			int test = 3;
			
			words = explode(remains[(i+1)..], " ");
			target = remains[0..i];
			remains = "";
			while(sizeof(words)) {
			    mud = implode(words, " ");
			    mud = INTERMUD_D->GetMudName(lower_case(mud));
			    if( mud ) {
				target += mud;
				break;
			    }
			    if( remains == "" ) {
				remains = words[<1];
			    }
			    else {
				 remains = words[<1] + " " + remains;
			    }
			    words = words[0..<2];
			}
			if( !mud ) {
			    msg_data = SOUL_D->GetChannelEmote(emote_cmd,
							       "STR", remains);
			    target = 0;
			}
			else {
			    if( trim(remains) == "" ) {
				msg_data = SOUL_D->GetChannelEmote(emote_cmd,
								   "LIV");
			    }
			    else {
				msg_data = SOUL_D->GetChannelEmote(emote_cmd,
								   "LIV STR",
								   remains);
			    }
			    if( !msg_data ) {
				str = "$N " + str;
				target = 0;
			    }
			}
		    }
		}
	    }
	    if( msg_data ) {
		string sgen = this_player()->GetGender();
		string tgen = 0;

		if( ob ) {
		    target = ob->GetName();
		    tgen = ob->GetGender();
		}
		else if( target ) {
		    string user, mud;

		    sscanf(target, "%s@%s", user, mud);
		    targetkey = target;
		    tgen = SERVICES_D->GetRemoteGender(user, mud);
		    target = SERVICES_D->GetRemoteDisplayName(user, mud);
		    if( !target ) {
			target = capitalize(targetkey);
		    }
		}
		str = create_message(POV_OBSERVER, msg_data[0][0],
				     msg_data[0][1], "$N", sgen, "$O", tgen,
				     msg_data[1]);
		if( target ) {
		    target_msg = create_message(POV_TARGET, msg_data[0][0],
						msg_data[0][1], "$N", sgen,
						"$O", tgen, msg_data[1]);
		}
	    }
	    emote = 1;
	}
        else {
	    return 0;
	}
    }
    else {
	rc = GetRemoteChannel(verb);
    }
    if( member_array(this_player(), Channels[verb]) == -1 ) return 0;
    if( !str || str == "" ) {
        this_player()->SetBlocked(verb);
        return 1;
    }
    if( (int)this_player()->GetBlocked(verb) ) {
        if( (int)this_player()->GetBlocked("all") ) {
	    this_player()->eventPrint("You cannot chat while totally blocked.",
				      MSG_ERROR);
            return 1;
        }
        this_player()->SetBlocked(verb);
    }
    if( verb == "admin" || verb=="cre" ) {
        if( !(name = (string)this_player()->GetCapName()) )
          name = capitalize((string)this_player()->GetKeyName());
    }
    else name = (string)this_player()->GetName();
    if( target_msg ) {
	target_msg = replace_string(target_msg, "$O's", "your");
    }
    eventSendChannel(name, verb, str, emote, target, target_msg);
    if( rc != verb ) {
	if( ob ) {
	    SERVICES_D->eventSendChannel(name, rc, str, emote, target,
					 target_msg);
	}
	else {
	    SERVICES_D->eventSendChannel(name, rc, str, emote, targetkey,
					 target_msg);	    
	}
    }
    return 1;
}

varargs void eventSendChannel(string who, string ch, string msg, int emote,
			      string target, string targmsg) {
    if( file_name(previous_object()) == SERVICES_D) {
        ch = GetLocalChannel(ch);
        if( emote ) msg = replace_string(msg, "$N", who);
    }
    else if( origin() != ORIGIN_LOCAL && previous_object() != master() &&
	    file_name(previous_object()) != PARTY_D ) return;
    if( !Channels[ch] ) return;
    if( emote ) {
	object *obs;
	object ob;
	string this_msg, tmp;
	int len;
	
	if( target && (ob = find_player(convert_name(target))) ) {
	    target = (string)ob->GetName();
	}
	this_msg = "%^MAGENTA%^<" + ch + ">%^RESET%^ ";
	msg = replace_string(msg, "$N", who);
	if( target ) {
	    msg = replace_string(msg, "$O", target);
	    targmsg = replace_string(targmsg, "$N", who);
	    targmsg = capitalize(replace_string(targmsg, "$O", "you"));
	}
        obs = filter(Channels[ch], (: $1 && !((int)$1->GetBlocked($(ch))) :));
	tmp = this_msg + msg;
	foreach(object listener in obs) {
	    if( listener == ob ) continue;
	    listener->eventPrint(tmp, MSG_CONV);
	}
	if( member_array(ob, obs) != -1 ) {
	    if( ob && !((int)ob->GetBlocked(ch)) ) {
		tmp = this_msg + targmsg;
		ob->eventPrint(tmp, MSG_CONV);
	    }
	}
    }
    else {
	object *obs;
	
	msg = who + " %^MAGENTA%^<" + ch + ">%^RESET%^ " + msg;
        obs = filter(Channels[ch], (: $1 && !((int)$1->GetBlocked($(ch))) :));
	obs->eventPrint(msg, MSG_CONV);
    }
}

string *GetChannelList(string ch) {
    string *ret;
    object who;
    string list;
    int i;

    if( file_name(previous_object()) == SERVICES_D ) ch = GetLocalChannel(ch);
    else if( origin() != ORIGIN_LOCAL ) return ({});
    if( !Channels[ch] ) return ({});
    ret = ({});
    foreach(who in Channels[ch]) {
	if( !who || (int)who->GetInvis() || (int)who->GetBlocked(ch) )
	  continue;
	ret += ({ (string)who->GetName() });
    }
    return ret;
}

string GetLocalChannel(string ch) {
    switch(ch) {
	case "imud_code":
	return "intercre";

	case "imud_gossip":
	return "intergossip";
    }
    return ch;
}

string GetRemoteChannel(string ch) {
    switch(ch) {
	case "intercre": 
	return "imud_code";

	case "intergossip":
	return "imud_gossip";

        case "foundation":
        return "ie_flibcode";
    }
    return ch;
}

string *GetChannels() { return copy(keys(Channels)); }
