#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    int status = this_player()->GetProperty("debug");
    if(!str && !status) str = "on";
    else if(!str && status) str = "off";
    if(str != "on" && str != "off") return "Try: help debug";
    if(str == "on"){
        write("You enable debugging.");
        this_player()->SetProperty("debug", 1);
        return 1;
    }
    write("You disable debugging.");
    this_player()->SetProperty("debug", 0);
    return 1;
}

string GetHelp() {
    return ("Syntax: debug [on | off]\n\n"
            "Allows you to receive debugging information.");
}                                                    
