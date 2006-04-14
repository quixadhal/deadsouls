/**
 * modification of code contributed by daelas
 * started 2006-04-04
 */
#include <lib.h>
#include <daemons.h>
#include <stargate.h>

// inherit LIB_DAEMON;
inherit LIB_STARGATE;

mixed cmd(string str) 
{
    mapping network;

    if (!archp(previous_object())) return 0;

    network = STARGATE_D->getStargates();
    if (str) 
    {
	if (network[lower_case(str)])
	{
	    write(str+" is part of the Stargate Network.\n");
	}
	else 
	{
	    write(str+" is not currently part of the Stargate Network.\n");
	}

    }
    else 
    {
	write("These are the current Stargate locations in the Network:\n");
	if (!sizeof(network)) 
	{
	    message("info", "No locations found.", this_player());
	}
	else 
	{
	    string buf = "";
	    string name;
	    class stargate info;

	    foreach (name, info in network)
	    {
		buf += sprintf("%:-15s %:-10s %s\n", name, info->status, info->destination);
	    }
	    write(buf);
	}
    }
    return 1;
}
