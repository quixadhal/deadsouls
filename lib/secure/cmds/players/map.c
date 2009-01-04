#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string ret;
    int i = atoi(args);
    ret = MAP_D->GetMap(environment(this_player()), i);
    write(ret);
    return 1; 
}

void help() {
    message("help", "Syntax: <map>\n\n"
            "Displays a simple map of your location.\n\n", this_player());
}
