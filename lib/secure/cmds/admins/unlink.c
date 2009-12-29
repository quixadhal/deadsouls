/*    /secure/cmds/admins/unlink.c
 *    from the Dead Souls Mud Library
 *    unlink PLAYER
 *    created by Descartes of Borg 951216
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string primary, secondary;
    mixed tmp;

    if( !archp(previous_object()) ) return 0;
    if( !args || args == "" ) return "Unlink whom from whom?";
    if( sscanf(args, "%s from %s", secondary, primary) != 2 )
        return "Unlink whom from whom?";
    tmp = CHARACTER_D->eventUnlink(primary, secondary);
    if( !tmp ) return "Unlink failed.";
    else if( tmp == 1 ){
        this_player(1)->eventPrint("Unlinked.");
        return 1;
    }
    else return tmp;
}

string GetHelp(){
    return ("Syntax: unlink <CHARACTER> from <PRIMARY>\n\n"
            "Unlinks the named primary or secondary player from its "
            "primary character.  If the player you are unlinking is "
            "is itself a primary and has more than two secondaries, the "
            "first secondary will become a primary.  Otherwise the entire "
            "link will be removed.");
}
