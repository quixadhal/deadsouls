/*    /cmds/creator/which.c
 *    From the Dead Souls Mud Library
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
    string array dirs;
    mixed *aa;
    string dir, msg = "", ret;
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
    else dirs = CMD_D->GetPaths(args);
    if(sizeof(dirs)){
        msg += args + ":";
        foreach(dir in dirs) {
            if(isverb) msg += "\n\t" + dir;
            else msg += "\n\t" + dir + "/" + args + ".c";
        }
    }
    aa = filter(this_player()->GetCommands(), (: $1[0] == $(args) :) );
    if(sizeof(aa)){
        msg += "\n";
        msg += args + " is an add_action() defined in "+identify(aa[0][2]);
    }
    ret = this_player()->GetAlias(args);
    if(ret){
        msg += "\n"+ args + " is an alias that expands to: "+ret;
    }
    if( (ret = this_player()->GetXverb(args)) ){
        msg += "\n"+ args + " is an xverb that expands to: "+ret;
    }
    if(member_array(args, SOUL_D->GetEmotes()) != -1){
        msg += "\n"+ args + " is an emote, or 'feeling'.";
    }
    if(!sizeof(msg)) msg = args +": not found.";
    previous_object()->eventPrint(msg, MSG_SYSTEM);
    return 1;
}

string GetHelp() {
    return ("Syntax: which [command]\n\n"
            "Gives you a listing of all locations in which a command "
            "object may be found for you.\n"
            "See also: help, man, help");
}
