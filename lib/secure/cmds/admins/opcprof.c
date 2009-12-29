#include <lib.h>
#include <dirs.h>

inherit LIB_DAEMON;

int cmd(string arg)
{
    if(!archp(previous_object())) return 0;
    unguarded( (: opcprof("/log/opcprof.out") :) );
    write("Please review /log/opcprof.out.efun and "+
            "/log/opcprof.out.eoper to see the output of this command.");

    return 1;
}

string GetHelp(){
    return ("Syntax: opcprof\n\n"
            "Puts a list of efuns and eoperators in /log/opcprof.out.*\n "
            "See man: opcprof\n"
            "See also: fdinfo, netstat, mstatus, cache, callouts, dumpallobj,");
}
