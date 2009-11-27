#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

varargs int cmd(string str){
    string tmp, thing, file, *inheriteds;
    object ob;
    if(!str || sscanf(str,"%s %s",file,thing) != 2){
        write("Syntax: inherits FILE THING");
        return 1;
    }

    ob = get_object(thing);
    if(!ob){
        write("Sorry I can't find that thing.");
        return 1;
    }
    thing = (ob->GetShort() || thing);

    if(last(file,2) == ".c") file = truncate(file,2);

    tmp = DEFINES_D->GetDefine(file);

    if(tmp) file = tmp;

    write("Checking " + identify(ob) + " for inheritance of " +
            file + " , just a moment...");
    if(file){
        if(inherits(file, ob)){
            write(thing+" inherits "+file+".");
            return 1;
        }
        inheriteds = deep_inherit_list(ob);
        foreach(string item in inheriteds){
            if(!grepp(item, file)) inheriteds -= ({ item });
        }
    }
    if(!sizeof(inheriteds)){
        write("No matches found. "+ thing +
                " does not appear to inherit that file.");
        return 1;
    }
    write("Exact match not found, but here are some possible matches: ");
    write(implode(inheriteds,"\n"));
    return 1;
}

string GetHelp(){
    return ("Syntax: inherits <FILE> <THING>\n\n"+
            "Tries to match FILE with the list of inherited files "+
            "for THING.\nSee also: showtree, findobj, findfun, showfuns");
}
