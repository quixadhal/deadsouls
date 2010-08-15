/*    /verbs/common/ask.c
 *    from the Dead Souls Object Library
 *    created by Blitz@Dead Souls
 */

#include <lib.h>
#include <talk_type.h>
#include "include/ask.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("ask");
    SetRules("LIV STR", "STR", "LIV to STR", "LIV for STR", "LIV about STR");
    SetErrorMessage("Ask what?  Or ask whom to do what?");
    SetHelp("Syntax: ask <LIVING> <SOMETHING>\n"
            "        ask <SOMETHING>\n"
            "        ask <LIVING> about <SOMETHING>\n"
            "        ask <LIVING> to <DO SOMETHING>\n"
            "        ask <LIVING> for <SOMETHING>\n\n"
            "Some npcs and perhaps some inanimate objects will respond "
            "when you query the target with a specific question.\n"
            "See also: reply, say, shout, speak, tell, whisper, yell");
}

mixed can_ask_liv_to_str(string str) { return 1; }

mixed can_ask_liv_for_str(string str) { return 1; }

mixed can_ask_liv_about_str(string str) { return 1; }

mixed can_ask_liv_str(string str) {
    string tmp;

    if( !str ) {
        str = "";
    }
    if( sscanf(str, "to %s", tmp) ) return 1;
    else if( sscanf(str, "for %s", tmp) ) return 1;
    else return this_player()->CanSpeak(/* ob */0, TALK_LOCAL, str);
}

mixed can_ask_str(string str) {
    return this_player()->CanSpeak(0, TALK_LOCAL, str);;
}

mixed do_ask_liv_to_str(object ob, string str) {
    string lang = this_player()->GetDefaultLanguage();
    //string pre = "In " + lang + ", ";
    string pre = "In ";
    int lvl;
    string msg = translate(str, lvl=(this_player()->GetLanguageLevel(lang)));
    if(lvl < 50) pre += "execrable "+lang+", ";
    else if(lvl < 60) pre += "very poor "+lang+", ";
    else if(lvl < 75) pre += "broken "+lang+", ";
    else if(lvl < 99) pre += "imperfect "+lang+", ";
    else pre += lang + ", ";
    msg = translate(msg, ob->GetLanguageLevel(lang));
    message("my_action", pre + "you ask "+ob->GetName()+" to "+str+".",
            this_player() );
    message("other_action", pre + this_player()->GetName()+" asks "
            "you to "+msg+".", ob);
    message("other_action", pre + this_player()->GetName()+" asks "+
            ob->GetName()+" to do something.",
            environment(ob), ({ ob, this_player() }) );
    ob->eventAsk(this_player(), str);
    return 1;
}

mixed do_ask_liv_for_str(object ob, string str) {
    string lang = this_player()->GetDefaultLanguage();
    string pre = "In " + lang + ", ";
    string msg = translate(str, this_player()->GetLanguageLevel(lang));
    msg = translate(msg, ob->GetLanguageLevel(lang));
    message("my_action", pre + "you ask "+ob->GetName()+" for "+str+".",
            this_player() );
    message("other_action", pre + this_player()->GetName()+" asks "+
            ob->GetName()+" for something.",
            environment(ob), ({ ob, this_player() }) );
    if( !(ob->eventRequest(this_player(), str)) )
        message("other_action", pre + this_player()->GetName()+" asks "
                "you for "+msg+".", ob);
    return 1;
}

mixed do_ask_liv_about_str(object ob, string str) {
    string lang = this_player()->GetDefaultLanguage();
    string pre = "In " + lang + ", ";
    string msg = translate(str, this_player()->GetLanguageLevel(lang));
    msg = translate(msg, ob->GetLanguageLevel(lang));
    message("my_action", pre + "you ask "+ob->GetName()+" about "+str+".",
            this_player() );
    message("other_action", pre + this_player()->GetName()+" asks "+
            ob->GetName()+" about something.",
            environment(ob), ({ ob, this_player() }) );
    if( !(ob->eventConsult(this_player(), str)) )
        message("other_action", pre + this_player()->GetName()+" asks "
                "you about "+msg+".", ob);
    return 1;
}

mixed do_ask_liv_str(object ob, string str) {
    mixed tmp;
    if( sscanf(str, "to %s", tmp) ) return do_ask_liv_to_str(ob, tmp);
    if( sscanf(str, "for %s", tmp) ) return do_ask_liv_for_str(ob, tmp);
    if( sscanf(str, "about %s", tmp) ) return do_ask_liv_about_str(ob, tmp);
    if( str[<1] != '?' ) str = capitalize(str) + "?";
    else str = capitalize(str);
    return this_player()->eventSpeak(ob, TALK_LOCAL, str);
}

mixed do_ask_str(string str) {
    if( str[<1] != '?' ) str = capitalize(str) + "?";
    else str = capitalize(str);
    return this_player()->eventSpeak(0, TALK_LOCAL, str);
    return 1;
}
