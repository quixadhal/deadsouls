/*    /cmds/adm/mfinger.c
 *    from the Dead Souls LPC Library
 *    a tool for fingering the site of a player
 *    created by Descartes of Borg 950429
 */

#include <lib.h>

inherit LIB_DAEMON;

private static mapping Waiting;

void ReadSocket(string str);

static void create() {
    daemon::create();
    Waiting = ([]);
}

mixed cmd(string args) {
    object ob;
    string ip;

    if( !args || args == "" ) return "Mfinger whom?";
    if( !(ob = find_player(args)) ) 
      return "No one around " + mud_name() + " by that name.";
    ip = query_ip_number(ob);
    args = (string)ob->GetCapName();
    ob = new(LIB_CLIENT);
    Waiting[ob] = ([ "who" : this_player(), "player" : args, "ip" : ip ]);
    if( (int)ob->eventCreateSocket(ip, 79) < 0 ) {
	message("error", "Unable to create socket.", this_player());
	ob->eventDestruct();
	return 1;
    }
    ob->SetDestructOnClose(1);
    ob->SetRead( (: ReadSocket :) );
    ob->eventWrite("\n");
    message("system", "Finger sent to " + possessive_noun(args) + " site " +
	    ip + ".", this_player());
    return 1;
}

void ReadSocket(string str) {
    object ob;

    if( !Waiting[ob = previous_object()] ) return;
    if( !Waiting[ob]["who"] ) return;
    message("system", "Information from " + Waiting[ob]["ip"] + " for " + 
	    Waiting[ob]["player"] + ":", Waiting[ob]["who"]);
    message("system", str, Waiting[ob]["who"]);
}

void help() {
    message("help", "Syntax: <mfinger [player]>\n\n"
	    "Allows you to get finger information from a player's site.",
	    this_player());
}
