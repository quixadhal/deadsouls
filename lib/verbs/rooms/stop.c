/*    /verbs/rooms/stop.c
 *    from the Dead Soulsr2 Object Library
 *    stop STR
 *    created by Descartes of Borg 951016
 */

#pragma save_binary

#include <lib.h>
#include "include/stop.h"

inherit LIB_DAEMON;

static void create() {
    daemon::create();
    SetNoClean(1);
    parse_init();
    parse_add_rule("stop", "STR");
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
