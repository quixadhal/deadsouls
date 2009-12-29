#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;
string funcname;
string *sourcefiles = ({});

mixed cmd(string args) {
    string ret, where, lcargs, tmp;
    int Efun, SEfun, subs;
    string *ret_arr, *lc_hits = ({}), *lc_tmp;

    if(!args || args == ""){
        write("You'll need to be more specific. Try: help findfun");
        return 1;
    }
    if(!strsrch(args, "-s ")){
        args = replace_string(args, "-s ", "");
        subs = 1;
    }
    if(!args || args == ""){
        write("You'll need to be more specific. Try: help findfun");
        return 1;
    }
    if(subs){
        ret_arr = FUNCTION_D->GetLCFunctions(args, 1);
        if(!sizeof(ret_arr)){
            write("No matches found.");
            return 1;
        }
        ret_arr = explode(format_page(ret_arr, 2), "\n");
        this_player()->eventPage(ret_arr);
        return 1;
    }
    lcargs = lower_case(args);
    lc_hits = FUNCTION_D->GetLCFunctions(lcargs);
    if(!strsrch(lcargs,"query")) tmp = replace_string(lcargs,"query","get"); 
    if(!strsrch(lcargs,"set_")) tmp = replace_string(lcargs,"set_","set"); 
    if(tmp) lc_tmp = FUNCTION_D->GetLCFunctions(tmp);
    if(!lc_hits) lc_hits = ({});
    if(sizeof(lc_tmp)) lc_hits += lc_tmp;
    if(!strsrch(lcargs,"get_")) tmp = replace_string(lcargs,"get_","get");
    if(tmp && !strsrch(tmp,"get_")) tmp = replace_string(tmp,"get_","get");
    if(tmp && !sizeof(lc_tmp)) lc_tmp = FUNCTION_D->GetLCFunctions(tmp);
    if(sizeof(lc_tmp)) lc_hits += lc_tmp;
    if(sizeof(lc_hits)) lc_hits -= ({ args });
    if(grepp(args,"-a ")){
        where = "/";
        args = replace_string(args,"-a ","");
    }
    else where = "/lib";
    funcname = args;
    ret = FUNCTION_D->GetInstances(args, where);
    write(ret);
    Efun = efun_exists(args);
    SEfun = sefun_exists(args);
    if(Efun && SEfun){
        write(args+" is an external driver function (efun) that is "+
                "overridden as a simulated efun (sefun) in the lib.");
    }
    else {
        if(Efun) write(args+" exists as an efun "+
                "(external fun in the driver).");
        if(SEfun) write(args+" exists as a sefun "
                "(simulated efun in the lib).");
    } 
    if(sizeof(lc_hits)){
        write("\nSee also: "+
                implode(sort_array(distinct_array(lc_hits), 1), ", ")+"\n");
    }
    return 1;
}

string GetHelp(){
    return ("Syntax: findfun <FUNCTION_NAME>\n"
            "        findfun -s <SUBSTRING>\n\n"
            "Tells you where a function is defined. With the "
            "substring flag (-s) it lists possible matching lfuns, "
            "efuns, and sefuns known to the command.\n"
            "\nExamples: \nfindfun AddHP\nfindfun -s class\n\n"
            "Note: When not doing a substring search, "
            "this command is case-sensitive.\n"
            "See also: showfuns, efun, efuns, sefun, sefuns, "
            "lfun, lfuns");
}
