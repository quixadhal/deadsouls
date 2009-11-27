#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

varargs int cmd(string str){
    mixed ret;
    if(!str){
        write("Try: help defines");
        return 1;
    }
    ret = DEFINES_D->GetDefine(str);
    write(str+": "+identify(ret));
    return 1;
}

string GetHelp(){
    return ("Syntax: defines <token>\n\n"
            "Returns the defined value for the speficied token, if "
            "it is defined in the default includes.\n"
            "Examples:\n"
            "define SEVERABLE_LIMBS\n"
            "define MAX_NEWBIE_LEVEL\n"
            "See also: types");
}
