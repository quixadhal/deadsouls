/*    /cmds/player/rwho.c
 *    from the Dead Souls LPC Library
 *    Sends an Intermud 3 who request
 *    created by Descartes of Borg 950623
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    if( !str ) return "Get a remote who from where?";
    if( !(str = (string)INTERMUD_D->GetMudName(str)) )
      return mud_name() + " is not aware of such a place.";
    SERVICES_D->eventSendWhoRequest(str);
    message("system", "Remote who query sent to " + str + ".", this_player());
    return 1;
}

void help() {
    write("Syntax: <rwho [mud]>\n\n"
      "Gives you a who list in abbreviated form from a mud on the\n"
      "global network following the CD protocols for intermud communication.\n"
      "The information given by the who varies from mud to mud.\n"
    );
}
