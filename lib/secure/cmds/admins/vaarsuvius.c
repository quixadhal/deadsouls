#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object tp = this_player();
    string tmpfile;
    string ret = "";
    int massacre, survivors = 0;
    object *obs;

    if(!tp || !archp(tp)) return "No.";

    if(args) args = replace_string(args,"\"","");

    if(!args) return "Try: help vaarsuvius";

    else obs = findobs(args);

    if(!massacre = sizeof(obs)) {
        write("No such objects found.");
        return 1;
    }

    foreach(object ob in obs){
        ob->eventDestruct();
        if(ob) destruct(ob);
        if(ob){
            survivors++;
            write(identify(ob)+" survived the purge.");
        }
    }
    if(!survivors){
        write("All "+massacre+" objects destructed.");
        return 1;
    }
    write("Of "+massacre+" targets, "+survivors+" survived the purge.");
    return 1;
}

string GetHelp(){
    return ("Syntax: vaarsuvius <THING>\n\n"
            "Destructs every THING found.\n"
            "See also: zap, dest, destfile");
}
