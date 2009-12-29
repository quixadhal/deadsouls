#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

string GetHelp();
mixed var, newval;
object ob;

int CheckVar(string var, object ob){
    mixed *vars;
    if(!var || !ob) return 0;
    vars = variables(ob);
    if(member_array(var, vars) == -1) return 0;
    return 1;
}

int cmd(string str) {
    int i;
    mixed ret;
    string cmd, what, tmpstr;

    if(!this_player() || !archp(this_player())){
        write("Sorry, this is an arch command.");
        return 1;
    }

    if(!str || str == ""){
        write(GetHelp());
        return 1;
    }

    i = sscanf(str,"%s %s %s", cmd, var, what);

    if(i != 3 || (cmd != "get" && cmd != "set")){
        write(GetHelp());
        return i;
    }

    if(cmd == "set"){
        int tmp;
        i = sscanf(what,"%s %s", newval, what);
        if(i != 2){
            write(GetHelp());
            return i;
        }
        if(sscanf(newval,"%d",tmp)) newval = tmp;
    }

    tmpstr = DEFINES_D->GetDefine(what);
    if(tmpstr) what = tmpstr;

    ob = get_object(what);

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

    if(!CheckVar(var, ob)){
        write("No such variable exists in that object.");
        return 1;
    }

    i = catch( ret = evaluate(bind( (: fetch_variable($(var)) :), ob)) );

    if(i){
        write("Error in variable query.");
        return 1;
    }

    write(var+" in "+identify(ob)+" is "+identify(ret));

    if(cmd == "get"){
        return 1;
    }

    if(cmd == "set"){
        evaluate(bind( (: store_variable($(var), $(newval)) :), ob));
        ret = evaluate(bind( (: fetch_variable($(var)) :), ob));
        write(var+" in "+identify(ob)+" is now "+identify(ret));
        return 1;
    }

    write("Error.");
    return 1;
}

string GetHelp(){
    return ("Syntax: var get <variable name> <object or file>\n"
            "        var set <variable name> <new value> <object or file>\n\n"
            "Sets or gets the value of a variable in an object.\n"
            "Examples:\n"
            "var get isPK me\n"
            "var set Attackable 0 fighter\n\n"
            "Use with EXTREME caution.");
}
