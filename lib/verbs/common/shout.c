/*    /verbs/common/shout.c
 *    from the Dead Souls  Object Library
 *    shout
 *    shout 
 *    let it all out
 *    created by Descartes of Borg 951119
 */

#include <lib.h>
#include <talk_type.h>
#include "include/shout.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("shout");
    SetRules("","in WRD STR","STR");
}

mixed can_shout() { return "Shout what?"; }

mixed can_shout_str(string str) {
    string lang;
    if( !str ) return 0;
    if(this_player()->GetGaggedType("local")) return 0;
    lang = this_player()->GetDefaultLanguage() || 
        this_player()->GetNativeLanguage();
    return can_shout_in_wrd_str(lang, str);
}

mixed can_shout_in_wrd_str(string lang, string str) {
    if( !lang || !str ) return 0;
    if( !creatorp(this_player()) && this_player()->GetStaminaPoints()<100 )
        return "You are too tired to shout.";
    if( strlen(str) > 250 ) return "That is way too much to shout!";
    return this_player()->CanSpeak(0, TALK_WORLD, str, lang);
}

mixed do_shout() { return 1; }

mixed do_shout_str(string str) {
    string lang;
    lang = this_player()->GetDefaultLanguage() || 
        this_player()->GetNativeLanguage();
    return do_shout_in_wrd_str(lang, str);
}

mixed do_shout_in_wrd_str(string lang, string str) {
    int cost = -100;
    if(this_player()->GetPlayerPaused()) cost = -300;
    if( str[<1] != '!' && str[<1] != '?' && str[<1] != '.' )
        str = capitalize(str) + ".";
    else str = capitalize(str);
    this_player()->AddStaminaPoints(cost);
    return this_player()->eventSpeak(0, TALK_WORLD, str, lang);
}

string GetHelp(string str) {
    return ("Syntax: shout <MESSAGE>\n"
            "        shout in <LANGUAGE> <MESSAGE>\n\n"
            "Sends a message to all players on " + mud_name() + ".  If you "
            "fail to specify a language, your native language is used.  "
            "Abuse of shouting is a violation of " + mud_name() + " laws.\n"
            "See also: reply, say, speak, tell, whisper, yell");
}
