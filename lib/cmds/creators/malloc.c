#include <lib.h>

inherit LIB_DAEMON;

int
cmd(string arg)
{
	malloc_status();
	return 1;
}

int help()
{
  write( @EndText
Syntax: malloc
Effect: Lists memory usage statistics
        Output will depend on memory management package specified in options.h
        when the driver is compiled.
See man: malloc_status
EndText
  );
  return 1;
}
