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
   
   if( !args || args == "" ) return "You must specify a command to execute.";
   before = rusage();
   catch(eval_cost = (int)previous_object()->eventForce(args));
   after = rusage();
   usertime = after["utime"] - before["utime"];
   stime = after["stime"] - before["stime"];
   message("system", "\n" + stime + " ms system time, " + usertime +
	   " ms user time, and " + eval_cost + " CPU cycles eval cost.",
	   this_player());
   return 1;
}

int help()
{
  write( @EndText
Syntax: gauge <command>
Effect: Gauges how many CPU cycles <command> takes to execute.
    and how much system and user time.
Nota Bene: <command> must be typed in full (no aliases)
EndText
  );
  return 1;
}
