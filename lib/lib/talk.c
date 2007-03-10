/*    /lib/talk.c
 *    from the Dead Souls  Object Library
 *    handles general yapping
 *    created by Descartes of Borg 951118
 *    Version: @(#) talk.c 1.5@(#)
 *    Last Modified: 96/10/08
 */

#include <talk_type.h>
#include <daemons.h>
#include <message_class.h>
#include "include/talk.h"
#define MAX_TELL_HIST_SIZE  50

private static string array TellHist = ({});
private string SpeakColor = "CYAN%^";

int GetPolyglot();

int direct_ask_liv_str() { return 1; }

int direct_ask_liv_to_str() { return 1; }

int direct_request_str_from_liv() { return 1; }

int direct_say_to_liv() { return 1; }

int direct_say_to_liv_str() { return 1; }

int direct_whisper_to_liv() { return 1; }

int direct_whisper_in_wrd_to_liv() { return 1; }

int direct_whisper_to_liv_str() { return 1; }

int direct_whisper_in_wrd_to_liv_str() { return 1; }

int direct_whisper_to_liv_in_wrd_str() { return 1; }

string SetSpeakColor(string str){
    if(!str) str = "foo";
    switch(str){
    case "red" : SpeakColor = upper_case(str)+"%^";break;
    case "green" : SpeakColor = upper_case(str)+"%^";break;
    case "orange" : SpeakColor = upper_case(str)+"%^";break;
    case "yellow" : SpeakColor = upper_case(str)+"%^";break;
    case "blue" : SpeakColor = upper_case(str)+"%^";break;
    case "magenta" : SpeakColor = upper_case(str)+"%^";break;
    case "black" : SpeakColor = upper_case(str)+"%^";break;
    case "white" : SpeakColor = upper_case(str)+"%^";break;
    case "b_red" : SpeakColor += upper_case(str)+"%^";break;
    case "b_green" : SpeakColor += upper_case(str)+"%^";break;
    case "b_orange" : SpeakColor += upper_case(str)+"%^";break;
    case "b_yellow" : SpeakColor += upper_case(str)+"%^";break;
    case "b_blue" : SpeakColor += upper_case(str)+"%^";break;
    case "b_magenta" : SpeakColor += upper_case(str)+"%^";break;
    case "b_black" : SpeakColor += upper_case(str)+"%^";break;
    case "b_white" : SpeakColor += upper_case(str)+"%^";break;
    case "b_cyan" : SpeakColor += upper_case(str)+"%^";break;
    case "flash" : SpeakColor += upper_case(str)+"%^";break;
    default : SpeakColor = "CYAN%^";break;
    }
    return SpeakColor;
}

string GetSpeakColor() { return SpeakColor; }

int eventTellHist(string str){
    string pob = base_name(previous_object());
    string stack = get_stack();
    if(pob != SERVICES_D && pob != "/secure/cmds/players/tell" &&
      stack != "get_stack eventTellHist eventHearTalk eventSpeak cmd cmdAll <function>" &&
      stack != "get_stack eventTellHist eventHearTalk eventSpeak cmd cmd cmdAll <function>"){
	return 0; 
    }
    if(sizeof(TellHist) > MAX_TELL_HIST_SIZE) TellHist -= ({ TellHist[0] });
    TellHist += ({ timestamp()+" "+str });
    return 1;
}

string array GetTellHistory(){
    if(this_object() != this_player() && !archp(this_player()))
	return ({});
    return copy(TellHist);
}

varargs mixed CanSpeak(object target, string verb, string msg, string lang) {
    if( lang && (!GetLanguageLevel(lang) || !GetLanguageName(lang)) )
	return "You don't speak that language!";
    if( target ) {
	if( target == this_object() )
	    return "Are you really intent on talking to yourself?";
	if( userp(target) && !interactive(target) )
	    return (string)target->GetName() + " is net-dead.";
	if( (int)target->GetBlocked("tell") )
	    return (string)target->GetName() + " is blocking all tells.";
    }
    return 1;
}

varargs mixed eventHearTalk(object who, object target, int cls, string verb,
  string msg, string lang) {
    string tmp;
    object *riders = this_object()->GetRiders();

    if(riders && sizeof(riders)) 
	riders->eventHearTalk(who, target, cls, verb, (msg ||0), (lang ||0));

    if( lang && !newbiep() && !GetPolyglot() ) msg = translate(msg, GetLanguageLevel(lang));
    switch(cls) {
    case TALK_PRIVATE:
	if( target != this_object() ) return 0;
	if( verb == "reply" )
	    tmp = "%^BOLD%^RED%^" + (string)who->GetName() +
	    " replies,%^RESET%^ \"" + msg + "%^RESET%^\"";
	else tmp = "%^BOLD%^RED%^" + (string)who->GetName() +
	    " tells you,%^RESET%^ \"" + msg + "%^RESET%^\"";
	if(target->GetMuffed() && member_array(who->GetKeyName(),target->GetMuffed()) == -1){
	    eventTellHist(tmp);
	    eventPrint(tmp, MSG_CONV);
	}
	break;

    case TALK_SEMI_PRIVATE:
	if( target != this_object() ) return 0;
	tmp = "%^BOLD%^" + SpeakColor + (string)who->GetName() + " whispers in " +
	capitalize(lang) + " to you,%^RESET%^ \"" + msg + "%^RESET%^\"";
	eventPrint(tmp, MSG_CONV);
	break;

    case TALK_LOCAL:
	if( target ) {
	    if( target != this_object() ) {
		if( msg[<1] == '?' ) tmp = (string)target->GetName();
		else tmp = "to " + (string)target->GetName();
		if( lang ) tmp += " in " + capitalize(lang);
	    }
	    else {
		if( msg[<1] == '?' ) tmp = "you";
		else tmp = "to you";
		if( lang ) tmp += " in " + capitalize(lang);
	    }
	    tmp = (string)who->GetName() + " " + pluralize(verb) + " " + tmp +
	    ", \"";
	    tmp = tmp + "%^BOLD%^CYAN%^\"" + msg + "%^RESET%^\"";
	    eventPrint(tmp, MSG_CONV);
	}
	else if( verb == "yell" ) {
	    tmp = "%^BOLD%^GREEN%^You hear a " + (string)who->GetGender()
	    + " " + (string)who->GetRace() + " yell in " + capitalize(lang) +
	    " from a distance,%^RESET%^ \"" + msg + "%^RESET%^\"";
	    eventPrint(tmp, MSG_CONV);
	}
	else {
	    if( lang ) tmp = pluralize(verb) + " in " + capitalize(lang);
	    else tmp = pluralize(verb);
	    tmp = (string)who->GetName() + " " + tmp + ", \"";
	    tmp = tmp + "%^BOLD%^" + SpeakColor + msg + "%^RESET%^\"";
	    eventPrint(tmp, MSG_CONV);
	}
	break;

    case TALK_AREA:
	tmp = "%^BOLD%^GREEN%^" + (string)who->GetName() + " yells in " +
	capitalize(lang) + ",%^RESET%^ \"" + msg + "%^RESET%^\"";
	eventPrint(tmp, MSG_CONV);
	break;

    case TALK_WORLD:
	tmp = "%^BOLD%^BLUE%^" + (string)who->GetName() + " shouts in " +
	capitalize(lang) + ",%^RESET%^ \"" + msg + "%^RESET%^\"";
	eventPrint(tmp, MSG_CONV); 
	break;

    default:
	return 0;
    }
    eventTalkRespond(who, target, cls, msg, lang);
    return 1;
}

mixed eventTalkRespond(object who, object targ, int cls, string msg, string lang) {
    return true(who,targ,cls,msg,lang);
}

varargs mixed eventSpeak(object target, int cls, string msg, string lang) {
    string verb, tmp;
    int cols;

    if( lang && !GetPolyglot() ) {
	msg = translate(msg, GetLanguageLevel(lang));
	lang = GetLanguageName(lang);
    }
    cols = GetScreen()[0];
    if( msg[<1] != '?' && msg[<1] != '!' && msg[<1] != '.' )
	msg = capitalize(msg) + ".";
    else msg = capitalize(msg);
    switch( cls ) {
    case TALK_PRIVATE:
	tmp = "%^BOLD%^RED%^You tell " + (string)target->GetName() +
	",%^RESET%^ \"" + msg + "%^RESET%^\"";
	eventPrint(tmp, MSG_CONV);	
	target->eventHearTalk(this_object(), target, cls, "tell", msg);
	eventTellHist(tmp);
	return 1;

    case TALK_SEMI_PRIVATE:
	if( !target ) tmp = "%^BOLD%^CYAN%^You whisper in " + capitalize(lang) +
	    ",%^RESET%^ \"" + msg + "%^RESET%^\"";
	else tmp = "%^BOLD%^CYAN%^You whisper in " + capitalize(lang) + " to " +
	    (string)target->GetName() + ",%^RESET%^ \"" + msg + "%^RESET%^\"";
	eventPrint(tmp, MSG_CONV);	
	environment()->eventHearTalk(this_object(), target, cls, "whisper",
	  msg, lang);
	return 1;

    case TALK_LOCAL:
	if( msg[<1] == '?' ) verb = GetMessage("ask") || "ask";
	else if( msg[<1] == '!' ) verb = GetMessage("exclaim") || "exclaim";
	else {
	    verb = GetMessage("say") || "say";
	    if ((msg[<1] >= 'a' && msg[<1] <= 'z') ||
	      (msg[<1] >= 'A' && msg[<1] <= 'Z')) msg = msg + ".";
	}
	if( target && msg[<1] == '?' ) {
	    tmp = "You " + verb + " " + (string)target->GetName();
	    if( lang ) tmp = tmp + " in " + capitalize(lang);
	}
	else if( target ) {
	    tmp = "You " + verb + " to " + (string)target->GetName();
	    if( lang ) tmp = tmp + " in " + capitalize(lang);
	}
	else {
	    tmp = "You " + verb;
	    if( lang ) tmp = tmp + " in " + capitalize(lang);
	}
	tmp = tmp + ", \"%^BOLD%^" + SpeakColor + msg + "%^RESET%^\"";
	eventPrint(tmp, MSG_CONV);
	environment()->eventHearTalk(this_object(), target, cls, verb, msg,
	  lang);
	return 1;

    case TALK_AREA:
	tmp = "%^BOLD%^GREEN%^You yell in " + capitalize(lang) + ",%^RESET%^ \"" +
	msg + "%^RESET%^\"";
	eventPrint(tmp, MSG_CONV);   	
	environment()->eventHearTalk(this_object(), target, cls, "yell", msg,
	  lang);
	break;

    case TALK_WORLD:
	tmp = "%^BOLD%^BLUE%^You shout in " + capitalize(lang) + ",%^RESET%^ \"" +
	msg + "%^RESET%^\"";
	eventPrint(tmp, MSG_CONV); 		
	(users() - ({ this_object() }))->eventHearTalk(this_object(), target,
	  cls,"shout", msg, lang);
	return 1;

    default:
	return 0;
    }
}
