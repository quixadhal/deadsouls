/*    /verbs/common/say.c
 *    from the Dead Souls  Object Library
 *    say
 *    sat to LIV
 *    say STR
 *    say to LIV STR
 *    created by Descartes of Borg 951118
 */

#pragma save_binary

#include <lib.h>
#include <talk_type.h>
#include "include/say.h"

inherit LIB_DAEMON;

static void create() {
    daemon::create();
    SetNoClean(1);
    parse_init();
    /* error conditions */
    parse_add_rule("say", "");
    parse_add_rule("say", "to LIV");
    /* actual verbs */
    parse_add_rule("say", "to LIV STR");
    parse_add_rule("say", "STR");
}

mixed can_say() { return "Say what?"; }

mixed can_say_to_liv(object ob) {
    if( !ob ) return 0;
    return "What is it you are trying to say to "+ (string)ob->GetName() + "?";
}

mixed can_say_to_liv_str(object targ, string str) {
    if( !targ || !str ) return 0;
    return (mixed)this_player()->CanSpeak(targ, TALK_LOCAL, str);
}

mixed can_say_str(string str) {
    if( !str ) return 0;
    return (mixed)this_player()->CanSpeak(0, TALK_LOCAL, str);
}

mixed do_say() { return 1; }

mixed do_say_to_liv(object ob) { return 1; }

mixed do_say_to_liv_str(object targ, string str) {
    return (mixed)this_player()->eventSpeak(targ, TALK_LOCAL, str);
}

mixed do_say_str(string str) { return do_say_to_liv_str(0, str); }

string GetHelp(string str) {
    return ("Syntax: <say MESSAGE>\n"
	    "        <say to LIVING MESSAGE>\n\n"
	    "Sends out a message that everyone in the room can see.  If you "
	    "specify a target, the target person is shown as being the target "
	    "of the message.\n\n"
	    "See also: shout, speak, reply, tell, whisper");
}
	    
    
		     
