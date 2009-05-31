#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    string ret = "";

    if(!str){
        write("See: help morse");
        return 1;
    }

    ret = morse(str);

    write("Output: "+ret);
    return 1;
}



string GetHelp(string str) {
    return "Syntax: morse <string>\n"
        "Renders the provided string as morse code.\n"
        "";
}
