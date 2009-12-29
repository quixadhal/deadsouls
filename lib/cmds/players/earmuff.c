#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    if(!str || str =="") {
        if(!sizeof(this_player()->GetMuffed())) write("You are ignoring no one.");
        else {
            write("You are ignoring the following name(s):\n");
            write(implode(filter(this_player()->GetMuffed(), (: capitalize($1) :)), "\n")+"\n");
        }
        return 1;
    }
    else this_player()->AddMuffed(str);
    write("You add "+capitalize(str)+" to your earmuffed list.");
    return 1;
}

string GetHelp() {
    return ("Syntax: earmuff\n"
            "        earmuff <name>\n"
            "        earmuff @<mud>\n\n"
            "This command allows you to ignore channel messages from the name specified.\n"
            "You can also earmuff all channel messages coming from a "
            "specific mud, for example: earmuff @Spammy Mud II\n"
            "See also: whomuffed, unmuff");
}

