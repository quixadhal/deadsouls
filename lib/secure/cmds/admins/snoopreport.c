// This command is only useful if you own an intermud channel

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string chan, mud;

    if(!this_player() || !archp(this_player())) return 0;

    SNOOP_D->Report();

    return 1;
}
