#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
#if (WIZMAP && GRID)
    int status = this_player()->GetProperty("wizmapping");
    string sstatus = "are";
    if(!status) sstatus = "are not";
    if(!args){
        write("You "+sstatus+" wizmapping.");
        return 1;
    }
    if(args == "on"){
        if(status) write("You are already wizmapping.");
        else write("You enable wizmapping.");
        this_player()->SetProperty("wizmapping", 1);
        return 1;
    }

    if(args == "off"){
        if(!status) write("You are already not wizmapping.");
        else write("You disable wizmapping.");
        this_player()->SetProperty("wizmapping", 0);
        return 1;
    }

    write("Try: help wizmap");
    return 1;
#else
    write("This feature is disabled.");
    return 1;
#endif
}

string GetHelp() {
    return ("Syntax: wizmap [on | off]\n\n"
            "If enabled, a simple map of your surroundings is displayed "
            "when your environment is described to you.\nSee also: "
            "env, terminal, brief, mute, gag, minimap.");
}
