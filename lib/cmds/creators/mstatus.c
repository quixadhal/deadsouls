#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string arg) {
    int flag = !!arg;
    this_player()->eventPage(explode(mud_status(flag), "\n"));
    return 1;
}

string GetHelp(string str) {
return (@EndText
Syntax: mstatus [<arg>]
Effect: Give you statistics on the driver and mudlib
If the optional argument is given, you also get:
  add_message statistics
  Hash table of living objects
  Function cache information
  Heart beat information
  Shared string hash table
  Call out information
  Object name hash table status
See man: mud_status
See also: netstat, fdinfo, cache, callouts, dumpallobj, opcprof
EndText);
}
