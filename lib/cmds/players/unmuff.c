#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    if(!sizeof(this_player()->GetMuffed())) return "You are ignoring no one.";
    else if(!str || str == "") return "Please be more specific.";
    else {
        string *iglist = this_player()->GetMuffed();
        if(member_array(lower_case(str),iglist) == -1) return "You aren't ignoring them.";
        iglist -= ({ lower_case(str) });
        this_player()->SetMuffed(iglist);
        write("You remove "+capitalize(str)+" from your earmuffed list.");
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: unmuff <name>\n\n"
            "This command allows you to stop ignoring channel messages from the name specified.\n"
            "See also: earmuff, whomuffed");
}

