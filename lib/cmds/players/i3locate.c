/*    /cmds/players/i3locate.c
 *    from the Dead Souls LPC Library
 *    Intermud 3 command to locate people on other MUDs
 *    created by Descartes of Borg 950624
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    if( !args || args == "" ) return "i3locate whom?";
    SERVICES_D->eventSendLocateRequest(convert_name(args));
    message("system", "i3locate query sent.", this_player());
    return 1;
}

string GetHelp() {
    return ("Syntax: i3locate [person]\n\n"
            "This command allows you to know on which MUDs connected "
            "through the Intermud 3 network have someone using the "
            "name you specify.  Note that this person may not actually be "
            "the person you think it is, as nothing prevents two different "
            "people from using the same name on different MUDs.\n"
            "See also: mail, mudlist, rwho, tell");
}
