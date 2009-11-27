#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    object thing;
    string file, content;

    if(!str){
        write("Try: help about");
        return 1;
    }

    thing = get_object(str);

    if(!thing){
        write("There's no such object available here.");
        return 1;
    }

    if(interactive(thing)){
        write("Player save files are not subject to examination "
                "with this command.");
        return 1;
    }

    file = lpc_file(base_name(thing));

    if(!file && thing->GetVirtual()){
        file = lpc_file(path_prefix(base_name(thing)));
    }

    if(file && file_exists(file)) content = read_file(file);

    if(!content){
        write("That data is not currently available to you.");
        return 1;
    }

    message( "info", file + "\n", this_player() );

    return this_player()->eventPage( explode(content, "\n"), 
            MSG_SYSTEM | MSG_NOCOLOUR);
}

string GetHelp(string str) {
    return ("Syntax: about <OBJECT>\n"
            "        about here\n\n"
            "Displays the code for the object, if the file is "
            "available.\nSee also: more, find, findobj");
}
