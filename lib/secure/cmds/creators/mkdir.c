/*
// This file is part of the TMI Mudlib distribution.
// Please include this header if you use this code.
// Written by Sulam(1-8-92)
// Help added by Brian (1/28/92)
*/

#include <lib.h>
inherit LIB_DAEMON;

int help();

int cmd(string str) {
	if( !str )
	{
                return help();
	}
	str = absolute_path(this_player()->query_cwd(), str);
	if( file_size(str) != -1 )
	{
		notify_fail("mkdir: "+str+": file already exists.\n");
		return 0;
	}
	if( (int)master()->valid_write(str, previous_object(), "rmdir") == 0 )
	{
		notify_fail(str+": Permission denied.\n");
		return 0;
	}
	write(mkdir(str) ? "Ok.\n" : str+": couldn't make directory.\n");
	return 1;
}

int
help() {
  write("Command: mkdir\nSyntax: mkdidr <directory>\n");
  write("This command makes a new directory with the name specified.\n"+
        "If no path information is supplied, th new directory will be\n"+
        "a sub directory of the present working directory.  For more\n"+
        "on specifying paths see help cd.\n");
  return 1;
}
/* EOF */
