/**
 * /cmds/creators/classblock.c
 * From the Dead Souls Mud Library
 * Allows a creator to tune in or out of all class chat lines.
 * created by Hanse@Dead Souls 930305
 * Version: @(#) classblock.c 1.3@(#)
 * Last modified: 96/10/09
 */

#include <lib.h>
#include <daemons.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    if( !args || (args != "on" && args != "off") ) {
        return "Syntax: classblock <on|off>";
    }
    if( args == "on" ) {
        foreach(string class_name in CLASSES_D->GetClasses()) {
            if( !previous_object()->GetBlocked(class_name) ) {
                previous_object()->SetBlocked(class_name);
            }
        }
        previous_object()->eventPrint("You are now blocking all class lines.",
                MSG_SYSTEM);
    }
    else {
        foreach(string class_name in CLASSES_D->GetClasses()) {
            if( previous_object()->GetBlocked(class_name) ) {
                previous_object()->SetBlocked(class_name);
            }
        }
        previous_object()->eventPrint("You are no longer blocking any class "
                "lines.", MSG_SYSTEM);
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: classblock <on|off>\n\n"
            "Allows you to make sure that you are blocking all or no "
            "class chat lines.\n"
            "See also: codeblock");
}
