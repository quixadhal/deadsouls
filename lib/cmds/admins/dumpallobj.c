#include <lib.h>

inherit LIB_DAEMON;

int
cmd(string arg)
{
  if(!archp(previous_object())) return 0;
        dumpallobj("/tmp/objects");
	return 1;
}

int help()
{
  write( @EndText
Syntax: dumpallobj
Effect: Puts a list of all currently loaded objects in /log/dumps/obj_dump
See man: dumpallobj
See also: callouts, cache, mstatus, netstat, fdinfo, opcprof
EndText
  );
  return 1;
}
