/*    /verbs/players/speak.c
 *    from the Dead Soulsr2 Object Library
 *    speak
 *    speak STR
 *    speak in WRD STR
 */

#pragma save_binary

#include <lib.h>
#include <talk_type.h>
#include "include/speak.h"

inherit LIB_DAEMON;

static void create() {
    daemon::create();
    SetNoClean(1);
    parse_init();
    parse_add_rule("speak", "");
    parse_add_rule("speak", "in WRD");
    parse_add_rule("speak", "in WRD STR");
    parse_add_rule("speak", "STR");
}

mixed can_speak() { return "Speak what? In what language?"; }

mixed can_speak_in_wrd(string str) {
    if( str ) return "What do you wish to say?";
    else return 0;
}

mixed can_speak_str(string str) {
    string lang;
    
    if( !str ) return 0;
    if( strlen(str) > 3 && str[0..2] == "in " ) return 0;
    lang = (string)this_player()->GetNativeLanguage() || "english";
    return (mixed)this_player()->CanSpeak(0, TALK_LOCAL, str, lang);
}

mixed can_speak_in_wrd_str(string lang, string str) {
    if( !lang || !str ) return 0;
    if( !environment(this_player()) ) return "You are nowhere right now.";
    return (mixed)this_player()->CanSpeak(0, TALK_LOCAL, str, lang);
}

mixed do_speak() { return 1; }

mixed do_speak_in_wrd(string str) { return 1; }

mixed do_speak_str(string str) {
    string lang;

    lang = (string)this_player()->GetNativeLanguage() || "english";
    return do_speak_in_wrd_str(lang, str);
}

mixed do_speak_in_wrd_str(string lang, string str) {
    if( str[<1] != '.' && str[<1] != '?' && str[<1] != '!' )
      str = capitalize(str) + ".";
    else str = capitalize(str);
    return (mixed)this_player()->eventSpeak(0, TALK_LOCAL, str, lang);
}

string GetHelp(string str) {
    return ("Syntax: <speak MESSAGE>\n"
	    "        <speak in LANGUAGE MESSAGE>\n\n"
	    "Sends the message you specify to all people in the same room "
	    "as you.  If you are an avatar, you have the ability to customize "
	    "the way these messages come out through the \"message\" "
	    "command.  If you fail to specify a language, your native "
	    "language will be used.\n\n"
	    "See also: message, say, shout, speak, tell");
}

