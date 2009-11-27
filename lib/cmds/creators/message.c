/*    /cmds/creator/message.c
 *    from the Foundation II LPC Library
 *    allows a person to modify their default messages
 *    created by Descartes of Borg 950428
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string type, msg;

    if( !args || args == "" ) {
        mapping msgs;
        string *cles;
        int i;
        string tmp = "";

        i = sizeof(cles = keys(msgs = this_player()->GetMessages()));
        while(i--) tmp += sprintf("%:-10s %s\n", cles[i], msgs[cles[i]]);
        message("system", tmp, this_player());
        return 1;
    }
    if( sscanf(args, "%s %s", type, msg) != 2) return "Set it to what?";
    if( !(this_player()->SetMessage(type, msg)) )
        return "Invalid message type.";
    message("system", "Message " + type + " changed to:\n" + msg, 
            this_player());
    return 1;
}

string GetHelp() {
    return ("Syntax: message <msg type> <message>\n\n"
            "Allows you to modify standard " + mud_name() + " messages.  "
            "The message types are come, leave, telin, telout, home, "
            "clone, dest, login, logout, say, ask, exclaim.  Not that "
            "with say, ask, and exclaim you only may enter in the verbs.  "
            "Depending on the message class, you may place such placeholders "
            "as:\n$M\t" + mud_name() + "\n$N\tYour name\n$D\tdirection\n"
            "$O\tobject");
}
