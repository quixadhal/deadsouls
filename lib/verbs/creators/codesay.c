/*    /verbs/common/codesay.c
 *    from the Dead Souls  Object Library
 *    codesay
 *    codesay STR
 */

#include <lib.h>
#include <talk_type.h>
#include <commands.h>
#include "include/codesay.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("codesay");
    SetRules("STR");
}

mixed can_codesay() { return "Say what?"; }

mixed can_codesay_str(string str) {
    if( !str ) return 0;
    else return 1;
}

varargs mixed do_codesay_str(string str) {
    string filename = DIR_TMP + "/" + this_player()->GetKeyName() + ".codesay";
    if( !str ) {
        write("Huh?");
        return 1;
    }
    if(!creatorp(this_player())) {
        write("This is a command for creators.");
        return 1;
    }
    if(strsrch(str,";") == -1){
        write("You codesay: "+"%^BOLD%^CYAN%^"+str+"%^RESET%^");
        say(this_player()->GetCapName()+" codesays: "+
                "%^BOLD%^CYAN%^"+str+"%^RESET%^");
    }
    else {
        str = replace_string(str, ";", ";\n");
        str = replace_string(str, "{", "{\n");
        str = replace_string(str, "}", "}\n");
        write_file(filename,str,1);
        write_file(filename+"_rule","I",1);
        load_object(CMD_LSED)->cmd(filename+"_rule "+filename);
        write("You codesay: ");
        say(this_player()->GetCapName()+" codesays: ");
        tell_room(environment(this_player()),"\n"+
                "%^BOLD%^CYAN%^"+read_file(filename)+"%^RESET%^");
    }
    return 1;
}

mixed do_codesay() { return do_codesay_str(); }

string GetHelp() {
    return ("Syntax: codesay <MESSAGE>\n\n"
            "Sends out a message that everyone in the room can see. "
            "This message will be stripped of normal punctuation so "
            "that text that needs to be seen literally does not "
            "cause confusion. "
            "If the message contains a semicolon, this command will attempt "
            "to format it in indented LPC style. "
            "\n"
            "See also: shout, speak, reply, tell, whisper");
}
