#include <function.h>

static private mapping Knock = ([]);

// abstract methods
string GetDefiniteShort();
// end abstract methods

varargs mixed GetKnock(string str){
    if( !str ){
        str = "default";
    }
    return Knock[str];
}

string array GetKnocks(){
    return keys(Knock);
}

mapping RemoveKnock(string item){ 
    map_delete(Knock, item); 
    return Knock;
}

varargs mapping SetKnock(mixed key, mixed desc){
    if( !key ){
        key = "default";
    }
    if( !desc ){
        if( mapp(key) ){
            Knock = expand_keys(key);
        }
        else {
            Knock["default"] = key;
        }
    }
    else {
        Knock[key] = desc;
    }
    return Knock;
}

varargs mixed CanKnock(object who, string component){
    return 1;
}

varargs mixed eventKnock(object who, mixed component){
    mixed val;

    if( !component ){
        val = Knock["default"];
    }
    else {
        val = Knock[component];
    }
    if( arrayp(val) ){
        val = val[query_night()];
    }
    if( stringp(val) ){
        object env;

        env = environment(who);
        who->eventPrint(val);
        if( component ){
            env->eventPrint(who->GetName() + " knocks on the " + component +
                    " on " + GetDefiniteShort() + ".", who);
        }
        else {
            env->eventPrint(who->GetName() + " knocks on " +
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

mixed direct_knock_obj(object target){
    return CanKnock(this_player());
}

mixed direct_knock_on_obj(object target){
    return direct_knock_obj(target);;
}

mixed direct_knock_str_on_obj(string str, object target){
    return CanKnock(this_player(), remove_article(lower_case(str)));
}

mixed direct_knock_on_str_on_obj(string str, object target){
    return direct_knock_str_on_obj(str,target);
}

