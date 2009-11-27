#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

string GetHelp();

mixed cmd(string str) {
    if(str == "every mud") {
        SERVICES_D->eventSendWhoRequest();
    }
    else {
        return GetHelp();
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: i3who every mud\n\n"
            "Queries each active mud for rwho information.\n\n"

            "%^RED%^BOLD%^WARNING:%^RESET%^ This command sends a rwho request to every mud connected to the Intermud-3 network. "
            "If you're sure you want to do it, type \"i3who every mud\". Otherwise, you're probably "
            "looking for the \"rwho\" command, which lists information for one specified mud.\n"

            "See also: rwho");
}                                                    
