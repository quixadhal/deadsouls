#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    int code;
    string tmp;
    string ret = "";

    if(str && !code = atoi(str)){
        write("That doesn't appear to be an integer.");
        return 1;
    }

    if(code) {
        ret = convert_ascii(code);
        write("The ASCII code "+code+" is: "+ret);
        return 1;
    }

    for(code = 33; code < 256; code++){
        tmp = convert_ascii(code);
        if(sizeof(tmp)) ret += "The ASCII code "+code+" is: "+tmp+"\n"; 
    }

    write(ret);
    return 1;
}

string GetHelp(string str) {
    return "Syntax: ascii <CODE>\n"
        "Provided an integer that corresponds to an ASCII code known to "
        "the mud, that character will be displayed. Note that the mud's "
        "understanding of these characters is imperfect, and your client's "
        "rendition may be unsatisfactory.\nWithout an argument, the command "
        "displays all ASCII codes known to the mud and their characters.\n "
        "";
}
