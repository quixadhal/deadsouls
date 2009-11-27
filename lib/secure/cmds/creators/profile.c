#include <lib.h>
#include <daemons.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
#ifdef __PROFILE_FUNCTIONS__
    mixed *funs;
    object ob;
    string tmp, ret = "";
    int err;

    if(!file_exists(lpc_file(str))){
        tmp = absolute_path(this_player()->query_cwd(), str);
    }
    if(tmp && file_exists(lpc_file(tmp))) str = tmp;
    else if(!(ob = find_object(str))) str = DEFINES_D->GetDefine(str);
    if(!ob && file_exists(lpc_file(str))) err = catch( ob = load_object(str) );
    if(err || !ob){
        write("Could not find or load that object.");
        return 1;
    }
    write("Found "+identify(ob));
    funs = function_profile(ob);
    foreach(mixed element in funs){
        mixed temp = sort_array(keys(element),1);
        foreach(mixed sub in temp){
            ret += sub+": "+element[sub]+" ";
        }
        ret += "\n";
    }
    write(ret);
    return 1;
#else
    write("This command depends on an efun that is not available.");
    return 1;
#endif
}

string GetHelp(){
    return ("Syntax: profile <file or object>\n\n"+
            "Provides a report of the given object's function "+
            "profile data. See \"man function_profile\" for details "+
            "of what this data is. This command requires that the "+
            "driver be compiled with the PROFILE_FUNCTIONS define.\n"+
            "See also: vars, var, variables");
}
