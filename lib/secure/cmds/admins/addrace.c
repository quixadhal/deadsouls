/*    /secure/cmds/admins/addrace.c
 *    from the NIghtmare V Object Library
 *    creates new races
 *    created by Descartes of Borg 960528
 *    Version: 1.2
 *    Last Modified: 97/01/03
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string file;
    int player;

    if( !args || args == "" ) return "Syntax: addrace <RACE> [1 | 0]";
    if( sscanf(args, "%s %d", file, player) != 2 ) {
        file = args;
    }
    if( !file_exists(file=DIR_SECURE_CFG "/races/" + file) )
        return "File not found: " + file;
    RACES_D->AddRace(file, player);
    previous_object()->eventPrint("Race added.");
    return 1;
}

string GetHelp(){
    return ("Syntax: addrace <RACE> [0 | 1]\n\n"
            "Allows you to add a new race to the racial data stored in the "
            "races daemon.  Specifically, you create a configuration file "
            "and then issue this command to load the new race into the "
            "races daemon.  The optional second argument specifies if this "
            "race is a player or not.  If it is omitted or it is 0, the race "
            "is assumed to be NPC-only.  If it is 1, then it is assumed to "
            "be usable by players as well.\n"
            "NOTE: The PLAYER_RACE setting in the race file overrides that flag.\n"
            "The format of the file is explained here:\n"
            "http://dead-souls.net/ds-creator-faq.html#2.45\n\n"
            "An example of a human exists in /secure/cfg/races/human.  "
            "You will notice that a human hs no special resistances.  The "
            "number of times resistances, stats, limbs, and fingers appear "
            "does not matter so long as they appear in the file in the order "
            "resistances, stats, limbs, fingers.  In addition, parent "
            "limbs need to be listed sometime *before* any child limbs.  "
            "For example, the right hand may come any time after the right "
            "arm, but it must come after the right arm.\n"
            "This system is admittedly rather complex, but it beats hard "
            "coding these values.\nExample: addrace murlok 0\n"
            "See also: addclass, addemote");
}
