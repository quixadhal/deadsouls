/*    /secure/daemon/players.c
 *    from the Dead Soulsr1 Object Library
 *    a player management system
 *    created by Descartes of Borg 950926
 *    Version: @(#) players.c 1.2@(#)
 *    Last modified: 96/10/12
 */

#include <lib.h>
#include <privs.h>
#include "players.h"

inherit LIB_DAEMON;

static void create() { }

varargs int RemovePlayer(string str) {
    object who;

    if( !str ) { /* this is for suicide and undead death */
	string file, backup;

	if( !(who = previous_object()) || creatorp(who) ) return 0;
	file = save_file((string)who->GetKeyName()) + __SAVE_EXTENSION__;
	backup = DIR_SECURE_SAVE "/backup/" + who->GetKeyName() + "." + time();
	return unguarded( (: rename($(file), $(backup)) :) );
    }
    else {
	string file;

	if( !user_exists(str) ) return 0;
	if( member_group(str, PRIV_SECURE) || member_group(str, PRIV_ASSIST) )
	  return 0;
	who = find_player(str);
	file = save_file(str) + __SAVE_EXTENSION__;
	if( who ) {
	    message("system", "You have been removed from " + mud_name() + ".",
		    who);
	    who->eventDestruct();
	}
	return rm(file); 
    }
}
