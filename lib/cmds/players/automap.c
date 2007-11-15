#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    int status = this_player()->GetProperty("automapping");
    string sstatus = "are";
    if(!status) sstatus = "are not";
    if(!args){
        write("You "+sstatus+" automapping.");
        return 1;
    }
    if(args == "on"){
        if(status) write("You are already automapping.");
        else write("You enable automapping.");
        this_player()->SetProperty("automapping", 1);
        return 1;
    }

    if(args == "off"){
        if(!status) write("You are already not automapping.");
        else write("You disable automapping.");
        this_player()->SetProperty("automapping", 0);
        return 1;
    }

    write("Try: help automap");
    return 1;

}

void help() {
    message("help", "Syntax: automap [on | off]\n\n"
      "If enabled, a simple map of your surroundings is displayed "
      "when your environment is described to you.\n\n", this_player());
}
