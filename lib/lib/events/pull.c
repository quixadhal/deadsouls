#include <function.h>

static private mapping Pull = ([]);

// abstract methods
string GetDefiniteShort();
// end abstract methods

varargs mixed GetPull(string str){
    if( !str ){
        str = "default";
    }
    return Pull[str];
}

string array GetPulls(){
    return keys(Pull);
}

mapping RemovePull(string item){ 
    map_delete(Pull, item); 
    return Pull;
}

varargs mapping SetPull(mixed key, mixed desc){
    if( !key ){
        key = "default";
    }
    if( !desc ){
        if( mapp(key) ){
            Pull = expand_keys(key);
        }
        else {
            Pull["default"] = key;
        }
    }
    else {
        Pull[key] = desc;
    }
    return Pull;
}

varargs mixed CanPull(object who, string component){
    mixed val;

    if( !component ){
        component = "default";
    }
    val = Pull[component];
    if( !val ){
        if( component == "default" ){
            return 0;
        }
        else {
            return "#There is no " + component + " on " +
                GetDefiniteShort() + ".";
        }
    }
    else return 1;
}

varargs mixed eventPull(object who, string component){
    mixed val;

    if( !component ){
        val = Pull["default"];
    }
    else {
        val = Pull[component];
    }
    if( arrayp(val) ){
        val = val[query_night()];
    }
    if( stringp(val) ){
        object env;

        env = environment(who);
        who->eventPrint(val);
        if( component ){
            env->eventPrint(who->GetName() + " pulls the " + component +
                    " on " + GetDefiniteShort() + ".", who);
        }
        else {
            env->eventPrint(who->GetName() + " pulls " +
                    GetDefiniteShort() + ".", who);
        }
        return 1;
    }
    else {
        if( functionp(val) & FP_OWNER_DESTED ){
            return "Error in evaluating functional.";
        }
        return evaluate(val, who, component);
    }
}

mixed direct_pull_obj(object target){
    return CanPull(this_player());
}

mixed direct_pull_str_on_obj(string str, object target){
    return CanPull(this_player(), remove_article(lower_case(str)));
}

mixed direct_pull_str_from_obj(string str, object target){
    return direct_pull_str_on_obj(str, target);
}
