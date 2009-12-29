/*
// This file is part of the TMI Mudlib distribution
// Please include this header if you use this code.
// Written by Sulam(1-8-92)
// Help added (1/28/92) by Brian
 */

#include <lib.h>
inherit LIB_DAEMON;

string GetHelp();

    mixed cmd(string str){
        if( !str )
        {
            return GetHelp();
        }
        str = absolute_path(this_player()->query_cwd(), str);
        switch( file_size(str) )
        {
            case -1:
                notify_fail("rmdir: "+str+": No such file.\n");
                return 0; break;
            case -2:
                break;
            default:
                notify_fail("rmdir: "+str+": not a directory.\n");
                return 0; break;
        }
        if( master()->valid_write(str, this_object(), "rmdir") == 0 )
        {
            notify_fail(str+": Permission denied.\n");
            return 0;
        }
        write(rmdir(str) ? "Ok.\n" : str+": couldn't remove directory.\n");
        return 1;
    }

string GetHelp(){
    return ("Syntax: rmdir <directory>\n\n"+
            "This command allows you to remove the specified directory. If "
            "the directory is not empty then the command will fail.");
}
