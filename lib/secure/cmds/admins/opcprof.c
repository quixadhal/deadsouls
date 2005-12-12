#include <lib.h>
#include <dirs.h>


inherit LIB_DAEMON;

int
cmd(string arg)
{
    if(!archp(previous_object())) return 0;
    unguarded( (: opcprof("/log/opcprof.out") :) );
    write("Please review /log/opcprof.out.efun and "+
      "/log/opcprof.out.eoper to see the output of this command.");

    return 1;
}

int help()
{
    write( @EndText
Syntax: opcprof
Effect: Puts a list of efuns and eoperators in /log/opcprof.out.*
See man: opcprof
See also: fdinfo, netstat, mstatus, cache, callouts, dumpallobj,
EndText
    );
    return 1;
}
