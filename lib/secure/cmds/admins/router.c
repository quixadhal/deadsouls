#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string subcmd, arg1, arg2;

    if(!archp(previous_object())) return 0;
    if(find_object(ROUTER_D))
	write("I3 router daemon is loaded.");
    else {
	write("I3 router daemon is not loaded.");
	return 1;
    }

    if(!args || args == ""){
	return 1;
    }

    if(args == "show"){
	write("Router config: \n"+ROUTER_D->get_info());
	return 1;
    }

    if(sscanf(args,"%s %s",subcmd, arg1) != 2){
	write("Invalid router subcommand.");
	return 1;
    }

    if(arg1 == "mudlist"){
	mapping mudlist = ROUTER_D->query_mudinfo();
	write("Mudlist: \n"+identify(mudlist));
    }


    write("Router command completed.");
    return 1;
}

string GetHelp(string args) {
    return ("Syntax: \n\n"
      "\n\n"
      "");
}
