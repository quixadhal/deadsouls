/*    /verbs/common/whisper.c
 *    from the Dead Souls  Object Library
 *    whisper
 *    whisper to LIV STR
 *    created by Descartes of Borg 951118
 */

#include <lib.h>
#include <talk_type.h>
#include "include/whisper.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("whisper");
    SetRules("to LIV", "in WRD", "in WRD to LIV", "to LIV STR",
	     "to LIV in WRD STR", "in WRD to LIV STR");
    SetErrorMessage("Whisper to whom what?");
    SetHelp("Syntax: <whisper to LIVING MESSAGE>\n"
	    "        <whisper to LIVING in LANGUAGE MESSAGE>\n"
	    "        <whisper in LANGUAGE to LIVING MESSAGE>\n\n"
	    "Privately sends a message to the targetted person in the "
	    "language you specify with a small chance of being overheard.  "
	    "If you fail to specify a language, your native language is "
	    "used.\n\n"
	    "See also: say, shout, speak, tell, yell");
}


mixed can_whisper_to_liv(object target) {
    return "What is it you are trying to whisper?";
}

mixed can_whisper_in_wrd(string lang) {
    return "Whisper to whom in " + lang + "?";
}

mixed can_whisper_in_wrd_to_liv(string wrd, object ob) {
    return "What do you mean to whisper?";
}

mixed can_whisper_to_liv_str(object targ, string str) {
    string lang;
    
    lang = (string)this_player()->GetNativeLanguage() || "english";
    return can_whisper_to_liv_in_wrd_str(targ, lang, str);
}

mixed can_whisper_in_wrd_to_liv_str(string lang, object targ, string str) {
    return can_whisper_to_liv_in_wrd_str(targ, lang, str);
}

mixed can_whisper_to_liv_in_wrd_str(object targ, string lang, string str) {
    return (mixed)this_player()->CanSpeak(targ, TALK_SEMI_PRIVATE, str, lang);
}

mixed do_whisper() { return 1; }

mixed do_whisper_to_liv(object ob) { return 1; }

mixed do_whisper_in_wrd(string wrd) { return 1; }

mixed do_whisper_in_wrd_to_liv(string str, object ob) { return 1; }

mixed do_whisper_to_liv_str(object targ, string str) {
    string lang;

    lang = (string)this_player()->GetNativeLanguage() || "english";
    return do_whisper_to_liv_in_wrd_str(targ, lang, str);
}

mixed do_whisper_in_wrd_to_liv_str(string lang, object targ, string str) {
    return do_whisper_to_liv_in_wrd_str(targ, lang, str);
}

mixed do_whisper_to_liv_in_wrd_str(object targ, string lang, string str) {
    if( str[<1] != '?' && str[<2] != '!' && str[<1] != '.' )
      str = capitalize(str) + ".";
    else str = capitalize(str);
    return (mixed)this_player()->eventSpeak(targ, TALK_SEMI_PRIVATE, str,lang);
}
