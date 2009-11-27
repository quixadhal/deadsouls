#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int eventNoob(){
    write("%^RED%^WHAT A NEW PLAYER SHOULD KNOW\n%^RESET%^");
    this_player()->eventPage("/doc/help/players/handbook");
    return 1;
}

mixed cmd(string str) {
    return eventNoob();
}

string GetHelp() {
    eventNoob();
}

