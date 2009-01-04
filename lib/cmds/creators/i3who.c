#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    //    foreach(string mud in INTERMUD_D->GetMuds()){
    SERVICES_D->eventSendWhoRequest();
    //    }
    return 1;
}


void help() {
    message("help", "Syntax: i3who\n\n"
            "Queries each active mud for rwho information.\n\n",
            this_player());
}                                                    
