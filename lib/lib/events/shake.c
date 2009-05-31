#include <function.h>

static private mapping Shake = ([]);

// abstract methods
string GetDefiniteShort();
// end abstract methods

varargs mixed GetShake(string str){
    if( !str ){
        str = "default";
    }
    return Shake[str];
}

string array GetShakes(){
    return keys(Shake);
}

mapping RemoveShake(string item){ 
    map_delete(Shake, item); 
    return Shake;
}

varargs mapping SetShake(mixed key, mixed desc){
    if( !key ){
        key = "default";
    }
    if( !desc ){
        if( mapp(key) ){
            Shake = expand_keys(key);
        }
        else {
            Shake["default"] = key;
        }
    }
    else {
        Shake[key] = desc;
    }
    return Shake;
}

varargs mixed CanShake(object who, string component){
    mixed val;

    if( !component ){
        component = "default";
    }
    val = Shake[component];
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

varargs mixed eventShake(object who, string component){
    mixed val;

    if( !component ){
        val = Shake["default"];
    }
    else {
        val = Shake[component];
    }
    if( arrayp(val) ){
        val = val[query_night()];
    }
    if( stringp(val) ){
        object env;

        env = environment(who);
        who->eventPrint(val);
        if( component ){
            env->eventPrint(who->GetName() + " shakes the " + component +
                    " on " + GetDefiniteShort() + ".", who);
        }
        else {
            env->eventPrint(who->GetName() + " shakes " +
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

mixed direct_shake_obj(object target){
    return CanShake(this_player());
}

mixed direct_shake_str_on_obj(string str, object target){
    return CanShake(this_player(), remove_article(lower_case(str)));
}
