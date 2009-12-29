/*    /cmds/creators/last.c
 *    From the Dead Souls Mud Library
 *    Shows the last however many people to login
 *    Created by Descartes of Borg 961118
 *    Version: @(#) last.c 1.1@(#)
 *    Last modified: 96/11/18
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string file, filename;
    string array lines;
    int x = to_int(args);

    if(archp(this_player())) filename = DIR_SECURE_LOGS + "/enter";
    else filename = DIR_LOGS + "/enter";
    file = read_file(filename);

    if( file ) {
        lines = explode(file, "\n");
    }
    else {
        lines = ({});
    }
    if( x < 0 ) {
        x = absolute_value(x);
    }
    if( x > 0 ) {
        int y = sizeof(lines);

        if( x <= y ) {
            lines = lines[(y-x)..];
        }
    }
    if( !sizeof(lines) ) {
        previous_object()->eventPrint(filename+" is empty.", MSG_SYSTEM);
    }
    else {
        previous_object()->eventPage(lines, MSG_SYSTEM);
    }
    return 1;
}

string GetHelp(){
    return ("Syntax: last [NUMBER]\n\n"
            "Gives you a list of the last bunch of people to login or logout "
            "from the enter log.");
}
