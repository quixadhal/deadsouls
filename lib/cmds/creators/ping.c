#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string target;
    if(!str || str == "") str = mud_name();
    target = INTERMUD_D->GetMudName(str);
    this_player()->SetProperty("pinging",1);
    INTERMUD_D->eventWrite(({ "auth-mud-req", 5, mud_name(), 0,target, 0 }));

    return 1;
}

void help() {
    message("help", "Syntax: ping <mudname>\n\n"
      "Pings a mud to test connectivity. Only pings to "
      "Dead Souls muds are supported and recommended.\n"
      "Note that you may receive a reply from a different "
      "mud than the one specified if someone else already "
      "initiated a ping, or if your mud's keepalive happens "
      "to be running at the moment of your command.\n\n",
      this_player());
}                                                    
