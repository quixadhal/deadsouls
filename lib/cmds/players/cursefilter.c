#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    int status;
    if(!sizeof(str)) return "Try: help cursefilter";
    status = this_player()->GetParanoia("cursefilter");
    if(str == "on" && status) return "It's already on!";
    if(str == "off" && !status) return "It's already off!";
    if(str == "on"){
        this_player()->SetParanoia("cursefilter", 1);
        write("Curse filter enabled.");
        return 1;
    }
    if(str == "off"){
        this_player()->SetParanoia("cursefilter", 0);
        write("Curse filter disabled.");
        return 1;
    }
    return "Type: help cursefilter";
}

string GetHelp() {
    return ("Syntax: cursefilter [ on | off ]\n\n"
            "If enabled, tries to shield you from some common "
            "discourteous words.\n"
            "See also: env");
}
