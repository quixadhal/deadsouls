/*
// A useful command from the people at Portals.
// I think Huthar wrote this one.
// 94-07-24 :  Pallando added utime and stime bits from TMI
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object act_ob;
    mapping before, after;
    int stime, usertime, eval_cost;

#ifndef __HAS_RUSAGE__
    write("This command depends on an efun that is not available.");
#else
    if( !args || args == "" ) return "You must specify a command to execute.";
    before = rusage();
    catch(eval_cost = previous_object()->eventForce(args));
    after = rusage();
    usertime = after["utime"] - before["utime"];
    stime = after["stime"] - before["stime"];
    message("system", "\n" + stime + " ms system time, " + usertime +
            " ms user time, and " + eval_cost + " CPU cycles eval cost.",
            this_player());
#endif
    return 1;
}

string GetHelp(){
    string ret= "Syntax: gauge <command>\n\n"
        "Gauges how many CPU cycles <command> takes to execute. "
        "and how much system and user time.\n"
        "Nota Bene: <command> must be typed in full (no aliases).\n";
    if(archp(this_player())) ret += "See also: usage";
    return ret;
}
