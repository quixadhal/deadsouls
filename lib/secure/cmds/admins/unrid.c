/*    /secure/cmds/admins/unrid.c
 *    created 960301 by Blitz@Dead Souls
 */

#include <lib.h>
#include <dirs.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string file, dest;

    if( !sizeof(str) ) return "Unrid whom?";
    str = convert_name(str);
    file = DIR_RID + "/" + str + __SAVE_EXTENSION__;
    if( !file_exists(file) ) 
      return (file + ": file not found.  Cannot unrid.");
    if( rename(file, save_file(str) + __SAVE_EXTENSION__) ) {
        this_player()->eventPrint("Unrid failed, security "
                       "violation logged.");
        log_file("security", "\n**** Unrid violation attempted\n"
                 "Target: " + file + "\nCall Stack: " +
                 sprintf("%O", previous_object(-1)));
        return 1;
    }
    this_player()->eventPrint("Unrid successful.");
    return 1;
}

string GetHelp(string str) {
    return "Syntax: unrid <user name>\n\n"
           "This command restores a character that was "
           "banished via the \"rid\" command.";
}

