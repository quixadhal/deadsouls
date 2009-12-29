/*  /cmds/creator/mraces.c
 *  A tool for cres which displays a listing of all preset npc races
 *  Blitz@NM-IVD
 */

#include <lib.h>
#include <daemons.h>

mixed cmd(string str) {
    string *types;

    types = load_object(RACES_D)->GetRaces();
    if( !sizeof(types) ) {
        return "No list at this time.";
    }
    types = sort_array(types, 1);
    message("help", format_page(types, 5), this_player() );
    return 1;
}

string GetHelp() {
    return ("Syntax: mraces\n\n"
            "A simple tool for creators which will list "
            "available npc races.");
}

