/*    /secure/cmds/creators/create.c
 *    from the Dead Soulsr2.2 Object Library
 *    create WRD WRD STR
 *    created by Descartes of Borg 951202
 */

#include <lib.h>
#include <objects.h>
#include "include/create.h"

mixed cmd(string args) {
    string file, cmd, tmp;
    
    if( !args || args == "" ) return "Create in which file what?";
    if( sscanf(args, "%s %s %s", file, cmd, tmp) != 3 ) {
	if( sscanf(args, "%s %s", file, cmd) != 2 )
	  return "Create in which file what?";
	else args = 0;
    }
    file = absolute_path((string)this_player()->query_cwd(), file);
    if( file[<2..] != ".c" ) file = file + ".c";
    switch(cmd) {
	case "room":
	CreateRoom(file, cmd);
	return 1;

	default:
	return "I don't know how to create " + add_article(cmd) + ".";
    }
}

static void CreateRoom(string file, string args) {
    object ob;

    if( !(ob = new(OBJ_ROOMMAKER)) ) {
	this_player()->eventPrint("Unable to create a room maker.");
	return;
    }
    ob->eventCreateRoom(file, args);
}

string GetHelp(string str) {
    return ("Syntax: <create FILE room>\n"
	    "        <create FILE room CLIMATE>\n\n"
	    "Allows you to use a menu interface to create an object.  "
	    "Currently only room making is supported.\n\n"
	    "See also: ed");
}
