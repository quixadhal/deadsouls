/*    /verbs/rooms/stop.c
 *    from the Dead Souls Object Library
 *    stop STR
 *    created by Descartes of Borg 951016
 */



#include <lib.h>
#include "include/stop.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("stop");
    SetRules("STR");
}

mixed can_stop_str(string str) { 
    object ob;
    mixed err;

    if( str != "fishing" ) return 0;
    if( !(ob = environment(this_player())) ) return 0;
    err = (mixed)ob->CanStop(this_player(), "fishing");
    return (err || "You aren't fishing!");
}

mixed do_stop_str(string str) {
    object ob;

    ob = environment(this_player());
    return (mixed)ob->eventStop(this_player(), "fishing");
}

string GetHelp(string str) {
    return ("Syntax: <stop fishing>\n\n"
      "If you are fishing, this command will stop you from fishing.\n\n"
      "See also: cast, fish");
}
