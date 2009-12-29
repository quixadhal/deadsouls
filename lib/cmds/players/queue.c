#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string *cmds;
    if(!sizeof(args)){
        write("Try: help queue");
        return 1;
    }
    cmds = explode(args, ";");
    foreach(string cmnd in cmds){
        if(sizeof(cmnd)){
            write("Queuing command: "+cmnd);
            this_player()->eventQueueCommand(cmnd);
        }
    }
    this_player()->eventExecuteQueuedCommands();
    return 1;
}

string GetHelp(){
    return ("Syntax: queue <command1;command2>\n\n"
            "Stacks commands to be executed in order.");
}
