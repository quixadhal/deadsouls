/*    /daemon/command.c
 *    from the Dead Soulsr1 Object Library
 *    a new commands daemon, much faster than the old
 *    created by Descartes of Borg 940119
 */

#include <lib.h>
#include <dirs.h>
#include "include/command.h"

inherit LIB_DAEMON;

private static mapping Commands;
private static string *Paths;

void create() {
    daemon::create();
    SetNoClean(1);
    Commands = ([]);
    Paths = ({});
    eventRehash( ({ DIR_PLAYER_CMDS, DIR_CREATOR_CMDS, 
		    DIR_SECURE_CREATOR_CMDS }) );
}

void eventRehash(mixed paths) {
    string path;

    if( stringp(paths) ) paths = ({ paths });
    else if( !pointerp(paths) ) return;
    foreach(path in paths) {
	string file;

	if( file_size(path) != -2 ) continue;
	foreach(file in get_dir(path + "/*.c")) {
	    string cmd;

	    cmd = file[0..<3];
	    if( pointerp(Commands[cmd]) ) Commands[cmd] += ({ path });
	    else Commands[cmd] = ({ path });
	}
	Paths = distinct_array(Paths + ({ path }));
    }
}

string GetCommand(string cmd, string *path) {
    string *tmp;

    if( Commands[cmd] && sizeof(tmp = (path & (string *)Commands[cmd])) )
      return sprintf("%s/%s", tmp[0], cmd);
    else {
        tmp = (path & Paths);
        if( sizeof(tmp = path - tmp) ) eventRehash(tmp);
        if( Commands[cmd] && sizeof(tmp = (path & (string *)Commands[cmd])) )
          return sprintf("%s/%s", tmp[0], cmd);
    }
    return 0;
}

varargs string *GetCommands(string path) {
    string *paths, *tmp;
    string cmd;

    if( !path ) return keys(Commands);
    tmp = ({ });
    foreach( cmd, paths in Commands) 
      if( member_array(path, paths) != -1 ) tmp += ({ cmd });
    return tmp;
}

varargs string *GetPaths(string cmd) { 
    if( cmd ) return Commands[cmd];
    else return Paths;
}


