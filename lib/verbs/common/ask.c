/*    /verbs/common/ask.c
 *    from the Dead Souls  Object Library
 *    created by Blitz@Dead Souls
 */
 
#pragma save_binary

#include <lib.h>
#include <talk_type.h>
#include "include/ask.h"
 
inherit LIB_VERB;
 
static void create() {
    verb::create();
    SetVerb("ask");
    SetRules("LIV STR", "STR", "LIV to STR", "LIV for STR");
    SetErrorMessage("Ask what?  Or ask whom to do what?");
    SetHelp("Syntax: <ask LIVING SOMETHING>\n"
	    "        <ask SOMETHING>\n"
	    "        <ask LIVING to DO SOMETHING>\n"
	    "        <ask LIVING for SOMTHING>\n\n"
            "Some npcs and perhaps some inanimate objects will respond "
            "when you query the target with a specific question.\n\n"
            "See also: reply, say, shout, speak, tell, whisper, yell");
}
 
mixed can_ask_liv_to_str(string str) { return 1; }
 
mixed can_ask_liv_for_str(string str) { return 1; }
 
mixed can_ask_liv_str(string str) {
    string tmp;

    if( !str ) {
	str = "";
    }
    if( sscanf(str, "to %s", tmp) ) return 1;
    else if( sscanf(str, "for %s", tmp) ) return 1;
    else return (mixed)this_player()->CanSpeak(/* ob */0, TALK_LOCAL, str);
}
 
mixed can_ask_str(string str) {
    return (mixed)this_player()->CanSpeak(0, TALK_LOCAL, str);;
}
 
mixed do_ask_liv_to_str(object ob, string str) {
    message("my_action", "You ask "+(string)ob->GetName()+" to "+str+".",
            this_player() );
    message("other_action", (string)this_player()->GetName()+" asks "
            "you to "+str+".", ob);
    message("other_action", (string)this_player()->GetName()+" asks "+
            (string)ob->GetName()+" to "+str+".",
            environment(ob), ({ ob, this_player() }) );
    ob->eventAsk(this_player(), str);
    return 1;
}
 
mixed do_ask_liv_for_str(object ob, string str) {
    message("my_action", "You ask "+(string)ob->GetName()+" for "+str+".",
            this_player() );
    message("other_action", (string)this_player()->GetName()+" asks "+
            (string)ob->GetName()+" for "+str+".",
            environment(ob), ({ ob, this_player() }) );
    if( !((int)ob->eventRequest(this_player(), str)) )
      message("other_action", (string)this_player()->GetName()+" asks "
	      "you for "+str+".", ob);
    return 1;
}
 
mixed do_ask_liv_str(object ob, string str) {
    mixed tmp;

    if( sscanf(str, "to %s", tmp) ) return do_ask_liv_to_str(ob, tmp);
    if( sscanf(str, "for %s", tmp) ) return do_ask_liv_for_str(ob, tmp);
    if( str[<1] != '?' ) str = capitalize(str) + "?";
    else str = capitalize(str);
    return (mixed)this_player()->eventSpeak(ob, TALK_LOCAL, str);
}
 
mixed do_ask_str(string str) {
    if( str[<1] != '?' ) str = capitalize(str) + "?";
    else str = capitalize(str);
    return (mixed)this_player()->eventSpeak(0, TALK_LOCAL, str);
    return 1;
}
