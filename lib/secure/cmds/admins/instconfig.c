#include <lib.h>
#include <daemons.h>
#include NETWORK_H
#include SECRETS_H

inherit LIB_DAEMON;

void validate(){
    if(!master()->valid_apply(({ "SECURE"})) || !this_player()){
        error("no");
    }
    if(!archp(this_player()) || this_player()->GetForced()){
        error("i said no");
    }
}

mixed cmd(string str){
    mixed ret;
    int i, port;
    string cmd, args, name, addy;
    validate();
    if(!sizeof(str)) {
        write("Try: help instconfig");
        return 1;
    }
    i = sscanf(str, "%s %s", cmd, args);
    if(!i) cmd = str;
    if(cmd == "create"){
        i = sscanf(args,"%s %s %d", name, addy, port);
        if(i != 3) i = sscanf(args,"%s %d", name, port);
        if(i < 2) {
            write("Try: help instconfig");
            return 1;
        }
        if(undefinedp(addy)) addy = "127.0.0.1";
        ret = INSTANCES_D->InstCreate(name, addy, port);
        if(!ret){
            write("Some sort of problem occurred.");
            return 1;
        }
        if(stringp(ret)) write(ret);
        else write("Done.");
        return 1;
    }

    if(cmd == "remove"){
        write("Trying to remove: "+args);
        if(member_array(args, INSTANCES_D->GetInstances()) == -1){
            write("No such instance found.");
            return 1;
        }
        ret = INSTANCES_D->InstDelete(args);
        if(intp(ret)){
            write("Removal of "+args+" successful.");
        }
        else {
            write(ret);
        }
        return 1;
    }
    if(cmd == "list"){
        mapping instdata = INSTANCES_D->GetInstData();
        mixed insts = sort_array(keys(instdata), 1);
        ret = "";
        foreach(mixed key in insts){
            if(!instdata[key]) continue;
            ret += key + " " + instdata[key]["port"] + " " +
                (instdata[key]["online"] ? "%^GREEN%^online" : "%^RED%^offline");
            ret += "%^RESET%^\n";
        }
        if(!sizeof(ret)){
            ret = "The instance daemon is aware of no configured instances.";
        }
        write(ret);   
        return 1;
    }
}

string GetHelp(){
    return ("Syntax: instconfig list\n"
            "        instconfig create <name> <port>\n"
            "        instconfig remove <name>\n\n"
            "Modifies your instance configuration. To add an instance "
            "named Alpha on port 5555, you would type:\n"
            "instconfig create Alpha 5555\n\n"
            "To remove it:\n"
            "instconfig remove Alpha\n\n"
            "See also: instances, admintool, mudconfig");
}

