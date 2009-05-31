#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

    mixed cmd(string str) {
        if( !sizeof(str) )
            return "Syntax: mudinfo <mudname>";
        return MUDINFO_D->PrintMudInfo(str);
    }

void help() {
    message("help",
            "Syntax: mudinfo <mudname>\n"
            "\n"
            "If available, this command presents detailed Intermud-3 "
            "information on the mud specified.\n",
            this_player() );
}

