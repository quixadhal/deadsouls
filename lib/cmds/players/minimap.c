#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
#if MINIMAP
    int status = this_player()->GetProperty("minimapping");
    string sstatus = "are";
    if(!status) sstatus = "are not";
    if(!args){
        write("You "+sstatus+" minimapping.");
        return 1;
    }
    if(args == "on"){
        if(status) write("You are already minimapping.");
        else write("You enable minimapping.");
        this_player()->SetProperty("minimapping", 1);
        return 1;
    }

    if(args == "off"){
        if(!status) write("You are already not minimapping.");
        else write("You disable minimapping.");
        this_player()->SetProperty("minimapping", 0);
        return 1;
    }

    write("Try: help minimap");
    return 1;
#else
    write("This feature is disabled.");
    return 1;
#endif
}

string GetHelp() {
    return ("Syntax: minimap [on | off]\n\n"
            "If enabled, a simple map of your surroundings is displayed "
            "when your environment is described to you.\nSee also: "
            "env, terminal, brief, mute, gag.");
}
