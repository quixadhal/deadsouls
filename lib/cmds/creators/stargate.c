/**
 * modification of code contributed by daelas
 * started 2006-04-04
 */
#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str){ 
    mapping network;

    network = STARGATE_D->GetStargates();
    if (str){ 
        if (network[lower_case(str)]){
            write(str+" is part of the Stargate Network.\n");
            write(str+" data: "+identify(network[lower_case(str)]));
        }
        else {
            write(str+" is not currently part of the Stargate Network.\n");
        }
        return 1;
    }
    else {
        write("These are the current Stargate locations in the Network:\n");
        if (!sizeof(network)){
            write("No locations found.");
            return 1;
        }
        else {
            string buf = "";
            string name;
            write(implode(keys(network), ", "));
        }
    }
    return 1;
}

void help() {
    message("help", "Syntax: stargate <gate>\n\n"
            "With an argument, this command will determine whether the argument "
            "supplied is a valid stargate. If so, stargate data on that gate "
            "is displayed. Without an argument, this command lists known valid "
            "stargates.",
            this_player());
}                                                    
