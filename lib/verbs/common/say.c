/*    /verbs/common/say.c
 *    from the Dead Souls  Object Library
 *    say
 *    sat to LIV
 *    say STR
 *    say to LIV STR
 *    created by Descartes of Borg 951118
 */

#include <lib.h>
#include <talk_type.h>
#include "include/say.h"

inherit LIB_VERB;

static void create() {
    ::create();
    SetVerb("say");
    SetRules("","to LIV STR","STR");
}

mixed can_say_to_liv(object ob) {
    if( !ob ) return 0;
    return "What is it you are trying to say to "+ ob->GetName() + "?";
}

mixed can_say_to_liv_str(object targ, string str) {
    string lang = this_player()->GetDefaultLanguage() || 
        this_player()->GetNativeLanguage();
    return this_player()->CanSpeak(targ, TALK_LOCAL, str, lang);
}

mixed can_say(mixed args...) {
    return 1;
}

mixed can_say_str(string str) {
    string lang = this_player()->GetDefaultLanguage() ||
        this_player()->GetNativeLanguage();
    if( !str ) return 0;
    return this_player()->CanSpeak(0, TALK_LOCAL, str, lang);
}

mixed do_say(mixed args...){
    write("Say what?");
    return "Say whut?";
}

mixed do_say_to_liv(object ob) { return 1; }

mixed do_say_to_liv_str(object targ, string str) {
    string lang = this_player()->GetDefaultLanguage() ||
        this_player()->GetNativeLanguage();
    return this_player()->eventSpeak(targ, TALK_LOCAL, str, lang);
}

mixed do_say_str(string str) { return do_say_to_liv_str(0, str); }

string GetHelp(string str) {
    return ("Syntax: say <MESSAGE>\n"
            "        say to <LIVING> <MESSAGE>\n\n"
            "Sends out a message that everyone in the room can see.  If you "
            "specify a target, the target person is shown as being the target "
            "of the message.\n"
            "See also: shout, speak, reply, tell, whisper");
}
