#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    object target;
    string ret = "Shadows found: \n";
    mapping retmap  = ([]);

    if(!str || str == "") str = "me";

    if(str == "me") str = this_player()->GetKeyName();
    if(!target = present(str, environment(this_player()))){
        write(capitalize(str)+" is not here.");
        return 1;
    }

    retmap = target->GetShadows();

    if(!sizeof(retmap)){
        write("No shadows found.");
        return 1;
    }

    foreach(mixed key, mixed val in retmap){
        string obbase;
        if(objectp(key)) obbase = base_name(key)+".c";
        else obbase = val;
        ret += obbase + "  " + val + "\n";
    }
    write(ret);
    return 1;
}

string GetHelp() {
    return ("Syntax: shadows <target>\n\n"
            "Identify shadows currently attached to an object.");
}                                                    
