/*    /cmds/creator/which.c
 *    From the Dead Souls Object Library
 *    gives you a list of directories in which a command may be found
 *    created by Descartes of Borg 950913
 *    Version: @(#) which.c 1.2@(#)
 *    Last modified: 96/10/08
 */

#include <lib.h>
#include <daemons.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string  array dirs;
    string dir, msg;
    int isverb;
    dirs = ({});

    isverb = 0;

    if( !args ) {
        return "You want to know information on which command?";
    }
    if(member_array(args,keys(VERBS_D->GetVerbs())) != -1){
        dirs += ({ (VERBS_D->GetVerbs())[args] });
        isverb = 1;
    }
    else if( !(dirs = CMD_D->GetPaths(args)) ) {
        return args + ": not found.";
    }
    msg = args + ":";
    foreach(dir in dirs) {
        if(isverb) msg += "\n\t" + dir;
        else msg += "\n\t" + dir + "/" + args + ".c";
    }
    previous_object()->eventPrint(msg, MSG_SYSTEM);
    return 1;
}

string GetHelp() {
    return ("Syntax: <which [command]>\n\n"
      "Gives you a listing of all directories in which a command "
      "object may be found.\n\n"
      "See also: help, man");
}
