//      Tellblock
//      A wiz utility to block tells only by Gregon@Dead Souls
//      based on _codeblock.c by Descartes of Borg.
//      06Nov1993


#include <lib.h>

inherit LIB_DAEMON;

#define CHANNELS "bin/daemon/channels"

int cmd() {
    this_player()->SetBlocked("tell");
    if(this_player()->GetBlocked("tell")) write("Tellblock on.\n");
    else write("Tellblock off.\n");
    return 1;
}

string GetHelp() {
    return ("Syntax: tellblock\n\n"+
            "Toggles the blocking of tells on and off.");
}
