/*
// This file is part of the TMI Mudlib distribution.
// Please include this header if you use this code.
// Written by Sulam(1-8-92)
// Help added by Brian (1/28/92)
 */

#include <lib.h>
inherit LIB_DAEMON;

string GetHelp();

mixed cmd(string str) {
    if( !str )
    {
        return GetHelp();
    }
    str = absolute_path(this_player()->query_cwd(), str);
    if( file_size(str) != -1 )
    {
        notify_fail("mkdir: "+str+": file already exists.\n");
        return 0;
    }
    if( master()->valid_write(str, previous_object(), "rmdir") == 0 )
    {
        notify_fail(str+": Permission denied.\n");
        return 0;
    }
    write(mkdir(str) ? "Ok.\n" : str+": couldn't make directory.\n");
    return 1;
}

string GetHelp(){
    return ("Syntax: mkdir <directory>\n\n"
            "This command makes a new directory with the name specified. "
            "If no path information is supplied, the new directory will be "
            "a sub directory of the present working directory. For more "
            "on specifying paths see: help cd");
}
