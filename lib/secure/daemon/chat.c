/*    /daemon/chat.c
 *    from the Nightmare 3.2 Mudlib
 *    daemon to handle all mud chat channels
 *    created by Descartes of Borg 931220
 */

#include <lib.h>
#include <config.h>
#include <pov.h>
#include <daemons.h>
#include <origin.h>
#include <message_class.h>
#include "include/chat.h"

inherit LIB_DAEMON;

string suspect,site,plainmsg,chan,foo,bar,baz;
static private mapping Channels;
static private mapping chanlast;

static private string *local_chans = ({"newbie","cre","gossip","admin","error",
  "priest", "mage", "explorer", "thief", "fighter", "death" });


static void create() {
    object pl;
    string *tmp_arr = ({});
    daemon::create();
    SetNoClean(1);
    Channels = ([]);
    foreach(pl in users()) {
	string *chans;
	string channel;

	if( pl && !(chans = (string *)pl->GetChannels()) ) continue;
	foreach(channel in chans) {
	    if( !Channels[channel] ) Channels[channel] = ({});
	    Channels[channel] = distinct_array(Channels[channel] + ({ pl }));
	}
    }
    foreach( string channel in local_chans ){
	tmp_arr += ({ channel + "emote" });
    }

    local_chans += tmp_arr;
}

varargs int CanListen(object who, string canal){
    //tc("CanListen: who: "+identify(who)+", canal: "+canal);
    if(RESTRICTED_INTERMUD == 0 || !RESTRICTED_INTERMUD) return 1;
    if(canal && member_array(canal, local_chans) != -1) return 1;
    else return imud_privp(who);
} 

varargs int CanTalk(object who, string canal){
    //tc("CanListen: who: "+identify(who)+", canal: "+canal);
    if(RESTRICTED_INTERMUD == 0 || !RESTRICTED_INTERMUD) return 1;
    if(canal && member_array(canal, local_chans) != -1) return 1;
    else return imud_privp(who);
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
	case "cre": case "intercre": case "intergossip":
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

int cmdLast(string feep){

    if(!chanlast||!Channels[feep]||member_array(this_player(), Channels[feep])==-1){

	this_player()->eventPrint(mud_name() + " does not have that channel on record.", MSG_ERROR);
	return 1;
    }
    if(!sizeof(chanlast[feep]))
    {
	this_player()->eventPrint("That channel has no backlog.", MSG_ERROR);
	return 1;
    }
    if(!CanListen(this_player(),feep)){
	write("You lack privileges to that channel.");
	return 1;
    }
    this_player()->eventPrint(implode(chanlast[feep], "\n"));
    return 1;
}

varargs int eventAddLast(string feep, string str, string pchan, string pmsg, string pwho)
{
    chan=feep;
    if(!chanlast)
	chanlast=([]);
    if(!sizeof(chanlast[chan]))
	chanlast[chan] = ({});
    if(sizeof(chanlast[chan]) == 50)
	chanlast[chan] = chanlast[chan][1..sizeof(chanlast[chan])];
    chanlast[chan] += ({ str });
    if(!pchan || pchan == "") pchan = "foo";
    plainmsg = "bar";
    if(pchan) plainmsg = "<" + pchan + "> ";
    if(pmsg) plainmsg += pmsg;
    if(pwho && pwho !="") plainmsg = pwho+" "+plainmsg;
    if(pchan && pchan != "admin"){
	unguarded( (: write_file("/log/chan/"+chan,"["+timestamp()+"] "+plainmsg+"\n") :) );
	if( file_size("/log/chan/"+chan) > 200000) {
	    unguarded( (: rename("/log/chan/"+chan,"/log/chan/"+chan+"."+timestamp() ) :) );
	}
    }
    else {
	unguarded( (: write_file("/secure/log/"+chan,"["+timestamp()+"] "+plainmsg+"\n") :) );
	if( file_size("/secure/log/"+chan) > 200000) {
	    unguarded( (: rename("/secure/log/"+chan,"/secure/log/"+chan+"."+timestamp() ) :) );
	}
    }
    return 1;
}

int cmdChannel(string verb, string str) {
    string varb, msg, name, rc, target, targetkey, target_msg;
    object ob = 0;
    int emote;

    if(first(str,1) == ":" && sizeof(str) > 3){
	if(!grepp(verb,"emote")) verb += "emote";
	str = trim(replace_string(str,":","",1));
    }

    if(grepp(verb, "emote")) varb = replace_string(verb,"emote","");
    else if(last(verb, 1) == ":") varb = replace_string(verb,":","");
    else varb = verb;

    if( verb == "hist" ) {
	if( !Channels[str] ) return 0;
	if( member_array(this_player(), Channels[str]) == -1 ) return 0;
	cmdLast(str);
	return 1;
    }

    if( verb == "list" ) {
	string *who;
	string ch, mud;

	if( !str ) {
	    return 0;
	}
	if( sscanf(str, "%s@%s", ch, mud) == 2 ) {
	    mud = trim(mud);
	    if(!alphap(last(mud,1))) mud = truncate(mud,1);
	    if( !Channels[ch] ) {
		return 0;
	    }
	    if( member_array(this_player(), Channels[ch]) == -1 ) {
		return 0;
	    }
	    if( ch == (ch = GetRemoteChannel(ch)) ) {
		if(!creatorp(this_player())){
		    write("Remote channel information is not available to players.");
		    return 1;
		}
	    }
	    if( !(mud = (string)INTERMUD_D->GetMudName(mud)) ) {
		this_player()->eventPrint(mud_name() + " is not aware of " 
		  "such a place.", MSG_ERROR);
		return 1;
	    }
	    if(!CanTalk(this_player(),verb)){
		write("You lack privileges to that channel.");
		return 1;
	    }
	    SERVICES_D->eventSendChannelWhoRequest(ch, mud);
	    this_player()->eventPrint("Remote listing request sent.",
	      MSG_SYSTEM);
	    return 1;
	}
	else ch = str;
	if( !Channels[ch] ) {
	    return 0;
	}
	if( member_array(this_player(), Channels[str]) == -1 ) {
	    return 0;
	}
	who = GetChannelList(str);
	msg = "Online: " + implode(who, "   ");
	//tc("msg1");
	this_player()->eventPrint(msg, MSG_SYSTEM);
	return 1;
    }
    if(!CanTalk(this_player(),verb)){
	write("You lack privileges to that channel.");
	return 1;
    }

    if( !Channels[verb] ) {
	if( sscanf(verb, "%semote", verb) || sscanf(verb, "%s:", verb) ) {
	    string emote_cmd, remains;
	    mixed array msg_data;
	    int i;
	    //tc("we seem to think it's an emote");
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

			words = explode(remains[(i+1)..], " ");
			target = remains[0..i];
			remains = "";
			while(sizeof(words)) {
			    mud = implode(words, " ");
			    mud = trim(mud);
			    if(!alphap(last(mud,1))) mud = truncate(mud,1);
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
	//this_player()->SetBlocked(verb);
	this_player()->eventPrint("Turn this channel on to talk on it.", MSG_ERROR);
	return 1;
    }
    if( verb == "admin" || verb=="cre" ) {
	if( !(name = (string)this_player()->GetCapName()) )
	    name = capitalize((string)this_player()->GetKeyName());
    }
    else name = (string)this_player()->GetName();
    if( target_msg ) {
	target_msg = replace_string(target_msg, "$O's", "your");
    }
    if(!grepp(str,"$N") && emote) str = "$N "+str;

    eventSendChannel(name, verb, str, emote, target, target_msg);
    if( ob ) {
	SERVICES_D->eventSendChannel(name, rc, str, emote, target,
	  target_msg);
    }
    else {
	SERVICES_D->eventSendChannel(name, rc, str, emote, targetkey,
	  target_msg);          
    }
    return 1;
}

varargs void eventSendChannel(string who, string ch, string msg, int emote,
  string target, string targmsg) {
    string pchan,pmsg;
    pchan=ch;

    if( file_name(previous_object()) == SERVICES_D) {
	ch = GetLocalChannel(ch);
	if( emote && sizeof(who)) msg = replace_string(msg, "$N", who);
    }
    else if( origin() != ORIGIN_LOCAL && previous_object() != master() &&
      file_name(previous_object()) != PARTY_D && ch != "death") return;
    if( !Channels[ch] ) return;
    if( emote ) {
	object *obs;
	object ob;
	string this_msg, tmp;
	if( target && (ob = find_player(convert_name(target))) ) {
	    target = (string)ob->GetName();
	}
	switch(ch)
	{
	case "death":
	    this_msg = "%^RED%^";
	    break;
	case "cre":
	    this_msg = "%^GREEN%^";
	    break;
	case "admin":
	    this_msg = "%^MAGENTA%^";
	    break;
	case "intergossip":
	    this_msg = "%^CYAN%^";
	    break;
	case "intercre":
	    this_msg = "%^YELLOW%^";
	    break;
	case "ds":
	    this_msg = "%^YELLOW%^";
	    break;
	case "newbie":
	    this_msg = "%^B_YELLOW%^";
	    break;
	case "gossip":
	    this_msg = "%^B_BLUE%^";
	    break;
	default:
	    this_msg = "%^BLUE%^";
	}
	this_msg += "<" + ch + ">%^RESET%^ ";
	msg = replace_string(msg, "$N", who);
	if( target ) {
	    msg = replace_string(msg, "$O", target);
	    targmsg = replace_string(targmsg, "$N", who);
	    targmsg = capitalize(replace_string(targmsg, "$O", "you"));
	}
	obs = filter(Channels[ch], (: $1 && !((int)$1->GetBlocked($(ch))) :));
	tmp = this_msg + msg;
	//ch = GetLocalChannel(ch);
	eventAddLast(ch, tmp, pchan, msg);
	foreach(object listener in obs) {
	    int ignore;
	    if(sscanf(who,"%s@%s",suspect,site) < 2) {
		suspect = who;
		site = "@"+mud_name();
	    }
	    else site = "@"+site;
	    if( listener == ob ) continue;
	    foreach(string jerk in listener->GetMuffed()){
		//tc("suspect: "+suspect);
		//tc("site: "+site);
		//tc("jerk: "+jerk);
		//tc("msg: "+msg);
		//sscanf(tmp,"%s %s %s", foo, bar, baz);
		//if(jerk && grepp(lower_case(bar),lower_case(jerk))) ignore = 1;
		if(jerk && lower_case(suspect) == lower_case(jerk)) ignore = 1;
		if(jerk && lower_case(site) == lower_case(jerk)) ignore = 1;
	    }
	    //tc("msg2","red");
	    if(!ignore && CanListen(listener,ch)) listener->eventPrint(tmp, MSG_CONV);
	    ignore = 0;
	}
	if( member_array(ob, obs) != -1 ) {
	    if( ob && !((int)ob->GetBlocked(ch)) ) {
		int ignore;
		tmp = this_msg + targmsg;
		foreach(string jerk in ob->GetMuffed()){
		    if(jerk && lower_case(suspect) == lower_case(jerk)) ignore = 1;
		    if(jerk && lower_case(site) == lower_case(jerk)) ignore = 1;
		}
		//tc("msg3","green");
		if(!ignore && CanListen(ob,ch)) ob->eventPrint(tmp, MSG_CONV);
		ignore = 0;
	    }
	}
	suspect = "";
	site = "";
    }
    else {
	object *obs;
	string tmsg;

	tmsg = who + " ";
	switch(ch)
	{
	case "cre":
	    tmsg += "%^GREEN%^";
	    break;
	case "death":
	    tmsg += "%^RED%^";
	    break;
	case "admin":
	    tmsg += "%^MAGENTA%^";
	    break;
	case "intergossip":
	    tmsg += "%^CYAN%^";
	    break;
	case "intercre":
	    tmsg += "%^YELLOW%^";
	    break;
	case "ds":
	    tmsg += "%^YELLOW%^";
	    break;
	case "newbie":
	    tmsg += "%^B_YELLOW%^";
	    break;
	case "gossip":
	    tmsg += "%^B_BLUE%^";
	    break;
	default:
	    tmsg += "%^BLUE%^";
	}

	tmsg += "<"+ch+">%^RESET%^ " + msg;
	pmsg = msg;
	msg = tmsg;
	//ch = GetLocalChannel(ch);
	eventAddLast(ch, msg, pchan, pmsg, who);
	obs = filter(Channels[ch], (: $1 && !((int)$1->GetBlocked($(ch))) :));
	foreach(object ob in obs){
	    int ignore;
	    if(sscanf(who,"%s@%s",suspect,site) < 2) {
		suspect = who;
		site = "@"+mud_name();
	    }
	    else site = "@"+site;

	    foreach(string jerk in ob->GetMuffed()){
		//if(jerk) tc("jerk: "+jerk,"red");
		if(jerk && lower_case(suspect) == lower_case(jerk)) ignore = 1;
		if(jerk && lower_case(site) == lower_case(jerk)) ignore = 1;
	    }
	    //tc("msg4","blue");
	    //tc("who: "+who);
	    //tc("suspect: "+suspect);
	    //tc("site: "+site);
	    if(!ignore && CanListen(ob,ch)) ob->eventPrint(msg, MSG_CONV);
	    ignore = 0;
	    suspect ="";
	    site = "";
	}

    }
}

string *GetChannelList(string ch) {
    string *ret;
    object who;

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

    case "dutch":
	return "dutch";

    case "dead_test4":
	return "ds_test";

    case "dead_souls":
	return "ds";

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

    case "dutch":
	return "dutch";

    case "ds_test":
	return "dead_test4";

    case "ds":
	return "dead_souls";

    }
    return ch;
}

string *GetChannels() { return copy(keys(Channels)); }
