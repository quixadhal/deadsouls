#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

void help();
object ob;
string ele;

int cmd(string str) {
    int i;
    mixed justvars, tmp;
    string cmd, what, ret = "";;

    if(!this_player() || !archp(this_player())){
        write("Sorry, this is an arch command.");
        return 1;
    }

    if(!str || str == ""){
        help();
        return 1;
    }

    tmp = DEFINES_D->GetDefine(str);
    if(tmp) str = tmp;

    what = str;
    ob = to_object(what);

    if(!ob){
        string path = this_player()->query_cwd()+"/";
        if(last(what,2) != ".c") what += ".c";
        if(file_exists(what)) ob = load_object(what);
        else if(file_exists(path+what)) ob = load_object(path+what);
    }

    if(!ob){
        write(truncate(what,2)+" not found.");
        return 1;
    }

    justvars = variables(ob);

    foreach(ele in justvars){
        i = catch( tmp = evaluate(bind( (: fetch_variable($(ele)) :), ob)) );
        if(!i) write(ele + " " + identify(tmp));
    }
    return 1;
}

void help() {
    write("Syntax: vars <object or file>\n"
            "Dumps the variables of an object as well as their values.\n"
            "Examples:\n"
            "vars kim\n"
            "vars /daemon/classes\n\n"
            "See also: var, variables");
}
