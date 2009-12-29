#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string target;
    if(!str || str == "") str = mud_name();
    target = INTERMUD_D->GetMudName(str);
    if(!target){
        write("Unknown target mud.");
        return 1;
    }
    this_player()->SetProperty("pinging",1);
    SERVICES_D->eventSendAuthRequest(target);
    return 1;
}

string GetHelp() {
    return ("Syntax: ping [mudname]\n\n"
            "Pings a mud to test this mud's connectivity. Only pings to "
            "Dead Souls muds are supported and recommended.\n"
            "Note that you may receive a reply from a different "
            "mud than the one specified if someone else already "
            "initiated a ping, or if your mud's keepalive happens "
            "to be running at the moment of your command.");
}                                                    
