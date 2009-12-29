/*    /verbs/creators/echo.c
 *    from the Dead Souls Mud Library
 *    echo
 *    echo STR
 *    echo to LIV STR
 *    created by Descartes of Borg 951114
 */

#include <lib.h>
#include <message_class.h>
#include "include/echo.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("echo");
    SetRules("","to LIV STR","STR");
}

int livings_are_remote() { return 1; }

mixed can_echo() {
    if( !creatorp(this_player()) ) return 0;
    else return "Echo what? Echo to whom what? Echo to all what?";
}

mixed can_echo_to_liv_str(object ob, string str) {
    if( !ob || !str ) return 0;
    if( !creatorp(this_player()) ) return 0;
    return 1;
}

mixed can_echo_str(string str) {
    if( !str || !creatorp(this_player()) ) return 0;
    return 1;
}

mixed do_echo() { return 1; }

mixed do_echo_to_liv_str(object who, string what) {
    eventEcho(({ who }), what);
    return 1;
}

mixed do_echo_str(string str) {
    object *obs;
    string tmp;

    if( sscanf(str, "to all %s", tmp) && tmp ) {
        obs = (users() - ({ this_player() }));
        str = tmp;
    }
    else obs = (all_inventory(environment(this_player())) - ({this_player()}));
    eventEcho(obs, str);
    return 1;
}

void eventEcho(object *targs, string str) {
    object ob;

    foreach(ob in targs) {
        if( archp(ob) )
            ob->eventPrint(this_player()->GetCapName() +
                    " echoes: " + str, MSG_CONV);
        else ob->eventPrint(str, MSG_CONV);
    }
    this_player()->eventPrint("You echo: " + str, MSG_CONV);
}

string GetHelp(string str) {
    return ("Syntax: echo <MESSAGE>\n"
            "        echo to <LIVING> <MESSAGE>\n"
            "        echo to all <MESSAGE>\n\n"
            "Sends a message without prefixing around to everyone in the "
            "room if you fail to specify to whom you are echoing.  You "
            "may echo to everyone in the game as well.  Abuse of this "
            "command is a severe violation of MUD ethics.\n"
            "See also: say, tell");
}
