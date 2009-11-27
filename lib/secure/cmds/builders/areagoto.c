#include <lib.h>

string GetHelp();

inherit LIB_DAEMON;

int cmd(string str) {
    if(!sizeof(str)){
        write(GetHelp());
        return 1;
    }

    str = homedir(this_player()) + "/area/room/" + str;
    if(last(str,2) != ".c") str += ".c";

    if(!directory_exists(homedir(this_player()) + "/area")){
        write(homedir(this_player()) + "/area/ does not exist!");
        return 1;
    }

    if(!file_exists(str)){
        write("The file "+str+" does not exist.");
        return 1;
    }

    if(strsrch(str,homedir(this_player()))){
        write("You can only use this command to travel to rooms "+
                "under your control.");
        return 1;
    }

    if(catch(load_object(truncate(str,2)))){
        write("There is a problem with that room. You stay where you are.");
        return 1;
    }

    this_player()->eventMoveLiving(str);

    return 1;
}

string GetHelp(){
    return ("Syntax: areagoto <room>\n\n"
            "Permits a builder to travel to rooms under her control. "
            "For a list of such rooms: arealist room\n"
            "See also: arealist, areaclone");
}
