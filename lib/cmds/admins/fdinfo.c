#include <lib.h>

inherit LIB_DAEMON;

int
cmd(string arg)
{
   if(!archp(previous_object())) return 0;
	dump_file_descriptors();
	return 1;
}

int help()
{
  write( @EndText
Syntax: fdinfo
Effect: Lists the status of the machines's file descriptors
See man: dump_file_descriptors
See also: netstat, mstatus, cache, callouts, dumpallobj, opcprof
EndText
  );
  return 1;
}
