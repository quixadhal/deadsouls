/*    /secure/cmds/admins/unrid.c
 *    created 960301 by Blitz@Dead Souls
 */

#include <lib.h>
#include <dirs.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string file;

    if(!archp(previous_object())) return 0;
    if( !sizeof(str) ) return "Unrid whom?";
    str = convert_name(str);
    file = save_file(DIR_RID + "/" + str);
    if( !file_exists(file) ) 
        return (file + ": file not found.  Cannot unrid.");
    if( rename(file, player_save_file(str)) ){
        this_player()->eventPrint("Unrid failed, security "
                "violation logged.");
        log_file("security", "\n**** Unrid violation attempted\n"
                "Target: " + file + "\nCall Stack: " +
                sprintf("%O", previous_object(-1)));
        return 1;
    }
    this_player()->eventPrint("Unrid successful.");
    PLAYERS_D->AddPlayerInfo(str);
    return 1;
}

string GetHelp(){
    return "Syntax: unrid <username>\n\n"
        "This command restores a character that was "
        "banished via the \"rid\" command.";
}

