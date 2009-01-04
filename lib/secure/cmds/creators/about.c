/*    /secure/cmds/creator/about.c
 *    Based on Descartes' <more>
 *    Created by Magnafix 9-8-96
 */

#include <lib.h>
#include <message_class.h>
#include "include/more.h"

inherit LIB_DAEMON;

mixed cmd(string str) {
    object thing;
    string name, tmp;
    if( !str || str == "" ) return "Syntax: <about OBJECT>";
    if(!present(str, this_player()) && str != "here" &&
            !present(str, environment(this_player())))
        return notify_fail("You don't see that.\n");
    if(str == "here") thing = environment(this_player());
    else thing = present( str, this_player());
    if(!thing) thing = present(str, environment(this_player()));
    if(thing->GetDoor()) thing = load_object(thing->GetDoor());
    if(!thing) return notify_fail("Uh oh, error.\n");
    if(interactive(thing)){
        message( "info","Players do not have files with LPC code. "
                "Player save files are not subject to examination with this command.",
                this_player());
        return 1;
    }
    name = base_name(thing) + ".c";
    message( "info", name + "\n", this_player() );
    return (mixed)this_player()->eventPage(name, MSG_SYSTEM | MSG_NOCOLOUR);
}

string GetHelp(string str) {
    return ("Syntax: <about OBJECT>\n\n"
            "Pages through the file referenced by the file name of "
            "the object that you name.  OBJECT can either be in "
            "your inventory or in the room.\nWhile inside the pager, you "
            "have access to the following commands:\n" +
            (string)LIB_PAGER->GetHelp("pager") + "\n\n"
            "See also: cat, ed, head, tail");
}
