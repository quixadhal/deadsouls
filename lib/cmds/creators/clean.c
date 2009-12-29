/*    /cmds/creator/clean.c
 *    From the Dead Souls Mud Library
 *    Cleans out the inventory of an object, defaulting to this room
 *    created by Valodin@Dead Souls 930101
 *    Version: @(#) clean.c 1.2@(#)
 *    Last modified: 96/10/06
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object array obs, items, users;
    object ob;

    if( !args || args == "" ) {
        ob = environment(previous_object());
    }
    else {
        ob = get_object(args);
    }
    if( !ob ) {
        return "No " + args + " found.";
    }
    previous_object()->eventPrint(sprintf("Cleaning %O", ob), MSG_SYSTEM);
    say(this_player()->GetName()+" cleans "+ob->GetShort()+".");
    obs = deep_inventory(ob);
    users = filter(obs, (: userp :));
    items = obs - users;
    foreach(object user in users) {
        object tmp = user;

        while( tmp = environment(tmp) ) {
            items -= ({ tmp });
        }
    }
    foreach(object item in items) {
        if( item ) {
            item->eventDestruct();
        }
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: clean [OBJECT]\n\n"
            "This command destroys all objects in the inventory of a "
            "specified object.  It skips any item that has a user somewhere "
            "in its deep inventory.");
}
