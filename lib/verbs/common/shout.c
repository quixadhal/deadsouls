/*    /verbs/common/shout.c
 *    from the Dead Souls  Object Library
 *    shout
 *    shout in WRD STR
 *    shout STR
 *    created by Descartes of Borg 951119
 */

#pragma save_binary

#include <lib.h>
#include <talk_type.h>
#include "include/shout.h"

inherit LIB_DAEMON;

static void create() {
    daemon::create();
    SetNoClean(1);
    parse_init();
    parse_add_rule("shout", "");
    parse_add_rule("shout", "in WRD STR");
    parse_add_rule("shout", "STR");
}

mixed can_shout() { return "Shout what?"; }

mixed can_shout_str(string str) {
    string lang;

    if( !str ) return 0;
    lang = (string)this_player()->GetNativeLanguage() || "english";
    return can_shout_in_wrd_str(lang, str);
}

mixed can_shout_in_wrd_str(string lang, string str) {
    if( !lang || !str ) return 0;
    if( !creatorp(this_player()) && (int)this_player()->GetStaminaPoints()<100 )
      return "You are too tired to shout.";
    if( strlen(str) > 250 ) return "That is way too much to shout!";
    return (mixed)this_player()->CanSpeak(0, TALK_WORLD, str, lang);
}

mixed do_shout() { return 1; }

mixed do_shout_str(string str) {
    string lang;

    lang = (string)this_player()->GetNativeLanguage() || "english";
    return do_shout_in_wrd_str(lang, str);
}

mixed do_shout_in_wrd_str(string lang, string str) {
    if( str[<1] != '!' && str[<1] != '?' && str[<1] != '.' )
      str = capitalize(str) + ".";
    else str = capitalize(str);
    this_player()->AddStaminaPoints(-100);
    return (mixed)this_player()->eventSpeak(0, TALK_WORLD, str, lang);
}

string GetHelp(string str) {
    return ("Syntax: <shout MESSAGE>\n"
	    "        <shout in LANGUAGE MESSAGE>\n\n"
	    "Sends a message to all players on " + mud_name() + ".  If you "
	    "fail to specify a language, your native language is used.  "
	    "Abuse of shouting is a violation of " + mud_name() + " laws.\n\n"
	    "See also: reply, say, speak, tell, whisper, yell");
}
