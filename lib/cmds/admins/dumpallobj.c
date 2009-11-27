#include <lib.h>

inherit LIB_DAEMON;

int cmd(string arg){
    if(!archp(previous_object())) return 0;
    dumpallobj("/tmp/objects");
    return 1;
}

string GetHelp(){
    return "Syntax: dumpallobj\n\n"
        "Puts a list of all currently loaded objects in /tmp/objects."
        "\nSee man: dumpallobj"
        "\nSee also: callouts, cache, mstatus, netstat, fdinfo, opcprof";
}
