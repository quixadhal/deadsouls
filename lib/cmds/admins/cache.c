#include <lib.h>

inherit LIB_DAEMON;

int cmd(string arg){
    if(!archp(previous_object())) return 0;
    cache_stats();
    return 1;
}

string GetHelp(){
    return "Syntax: cache\n\n"
        "Gets the cache stats from the driver, including the hit rate."
        "\nSee man: cache_stats"
        "\nSee also: callouts, mstatus, netstat, opcprof, fdinfo, dumpallobj";
}
