#include <function.h>

static private mapping Consult = ([]);

// abstract methods
string GetDefiniteShort();
// end abstract methods

varargs mixed GetConsult(string str){
    if( !str ){
        str = "default";
    }
    return Consult[str];
}

string array GetConsults(){
    return keys(Consult);
}

mapping RemoveConsult(string item){ 
    map_delete(Consult, item); 
    return Consult;
}

varargs mapping SetConsult(mixed key, mixed desc){
    if( !key ){
        key = "default";
    }
    if( !desc ){
        if( mapp(key) ){
            Consult = expand_keys(key);
        }
        else {
            Consult["default"] = key;
        }
    }
    else {
        Consult[key] = desc;
    }
    return Consult;
}

varargs mixed CanConsult(object who, string component){
    mixed val;

    if( !component ){
        component = "default";
    }
    val = Consult[component];
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

varargs mixed eventConsult(object who, string component){
    mixed val;

    if( !component ){
        val = Consult["default"];
    }
    else {
        val = Consult[component];
    }
    if( arrayp(val) ){
        val = val[query_night()];
    }
    if( stringp(val) ){
        object env;

        env = environment(who);
        who->eventPrint(val);
        if( component ){
            env->eventPrint(who->GetName() + " consults the " + component +
                    " on " + GetDefiniteShort() + ".", who);
        }
        else {
            env->eventPrint(who->GetName() + " consults " +
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

mixed direct_consult_obj(object target){
    return CanConsult(this_player());
}

mixed direct_consult_str_on_obj(string str, object target){
    return CanConsult(this_player(), remove_article(lower_case(str)));
}
