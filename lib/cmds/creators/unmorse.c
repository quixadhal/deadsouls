#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    string ret = "";

    if(!str){
        write("See: help morse");
        return 1;
    }

    ret = unmorse(str);

    write("Output: "+ret);
    return 1;
}

string GetHelp(){
    return "Syntax: unmorse <string>\n\n"
        "Decodes a morse string into its alphanumeric representation.\n"
        "See also: morse";
}
