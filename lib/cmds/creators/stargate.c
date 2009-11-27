#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str){ 
    mapping stargates;
    string *gates;
    string gatename;
    int err;
    object sgd;

    err = catch( sgd = load_object(STARGATE_D) );

    if(!sgd || err){
        write("There appears to be a problem with the stargate daemon.");
        return 1;
    }

    stargates = STARGATE_D->GetStargates();

    if(sizeof(stargates)) gates = keys(stargates);

    if(!gates){
        write("There are no known stargates.");
        return 1;
    }

    if(!str){
        write("Known stargates:");
        write(implode(gates, ", "));
        return 1;
    }

    gatename = lower_case(str);

    if(member_array(gatename, gates) == -1){
        write("Unknown stargate: \""+str+"\"");
        return 1;
    }

    write(str);
    foreach(mixed key, mixed val in stargates[gatename]){
        write(key + ": " + val);
    }

    return 1;
}

string GetHelp() {
    return ("Syntax: stargate [name]\n\n"
            "Without an argument, lists known stargates. If the name "
            "of a stargate is specified, information on it is listed.");
}                                                    
