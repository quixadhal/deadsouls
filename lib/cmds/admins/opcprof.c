#include <lib.h>


inherit LIB_DAEMON;

int
cmd(string arg)
{
   if(!archp(previous_object())) return 0;
#ifdef HAS_OPCPROF
	opcprof(LOG_DIR + "/dumps/opcprof");
#else
	message("my_action","Sorry, driver has been compiled with OPC_PROF disabled",this_player());
#endif
	return 1;
}

int help()
{
  write( @EndText
Syntax: opcprof
Effect: Puts a list of efuns and eoperators in /log/dumps/opcprof
See man: opcprof
See also: fdinfo, netstat, mstatus, cache, callouts, dumpallobj,
EndText
  );
  return 1;
}
