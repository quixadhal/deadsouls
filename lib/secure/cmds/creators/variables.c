#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

string *ret = ({});

string GetHelp();

int cmd(string str) {
    object ob;
    ret = ({});

    if(!str || str == ""){
        write(GetHelp());
        return 1;
    }

    ob = get_object(str);

    if(!ob){
        string path = this_player()->query_cwd()+"/";
        if(last(str,2) != ".c") str += ".c";
        if(file_exists(str)) ob = load_object(str);
        else if(file_exists(path+str)) ob = load_object(path+str);
    }

    if(!ob){
        write(str+" not found.");
        return 1;
    }

    filter( variables(ob,1), (: ret += ({ $1[1] + " " + $1[0] }) :) );

    ret = sort_array(ret, 1);

    write("List of variables for "+identify(ob)+": ");
    this_player()->eventPage(ret);

    return 1;
}

string GetHelp(){
    return ("Syntax: variables <object or file>\n\n"
            "Lists the names and type of variables in the named "
            "object or file.\n"
            "Examples: \n"
            "variables fighter\n"
            "variables workroom.c");
}
