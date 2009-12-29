#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    int i, lines = 2 * this_player()->GetScreen()[1];
    write("lines: "+lines);
    for(i=lines;i>0;i--){
        write("Loading memtest...\n");
    }
    this_player()->eventPage(explode(mud_status(1), "\n"));
    return 1;

}

string GetHelp() {
    return ("Syntax: mem\n\n"
            "This command makes it look like you're doing something "
            "important and technical that has to do with memory, but really "
            "it's just wiping your screen of game text.");
}
