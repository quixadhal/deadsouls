#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;
string funcname;
string *sourcefiles = ({});

mixed cmd(string args) {
    string ret, where;

    if(!args || args == ""){
        write("You'll need to be more specific. Try: help findfun");
        return 1;
    }
    if(grepp(args,"-a ")){
        where = "/";
        args = replace_string(args,"-a ","");
    }
    else where = "/lib";

    funcname = args;
    ret = FUNCTION_D->GetInstances(args, where);
    write(ret);
    return 1;
}

void help() {
    message("help", "Syntax: <findfun FUNCTION_NAME>\n\n"
      "Tells you which lib file a function is defined in.\n"
      "Example: findfun AddHP\n"
      "\n\n"
      "See also: doctool, showfuns", this_player());
}
