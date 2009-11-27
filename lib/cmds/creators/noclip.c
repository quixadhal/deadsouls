#include <lib.h>

inherit LIB_DAEMON;

varargs int cmd(string args){
    int status = this_player()->GetProperty("noclip");
    string sstatus = "are";
#if !(GRID)
    write("This feature is disabled.");
    return 1;
#endif
    if(!status) sstatus = "are not";
    if(!args){
        write("You "+sstatus+" noclipping.");
        return 1;
    }
    if(args == "on"){
        if(status) write("You are already noclipping.");
        else write("You enable noclip mode.");
        this_player()->SetProperty("noclip", 1);
        return 1;
    }

    if(args == "off"){
        if(!status) write("You are already not noclipping.");
        else write("You disable noclip mode.");
        this_player()->SetProperty("noclip", 0);
        return 1;
    }

    write("Try: help noclip");
    return 1;
}

string GetHelp(){
    return ("Syntax: noclip [ on | off ]\n\n"+
            "Allows you to travel to adjacent rooms known to the "+
            "mud, even if a door is in the way or there is no "+
            "explicit exit in that direction.\nSee also: "+
            "showgrid, wizmap, minimap, prox");
}
