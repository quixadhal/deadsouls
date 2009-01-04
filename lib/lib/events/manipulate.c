#include <function.h>

static private mapping Manipulate = ([]);

// abstract methods
string GetDefiniteShort();
// end abstract methods

varargs mixed GetManipulate(string str){
    if( !str ){
        str = "default";
    }
    return Manipulate[str];
}

string array GetManipulates(){
    return keys(Manipulate);
}

mapping RemoveManipulate(string item){ 
    map_delete(Manipulate, item); 
    return Manipulate;
}

varargs mapping SetManipulate(mixed key, mixed desc){
    if( !key ){
        key = "default";
    }
    if( !desc ){
        if( mapp(key) ){
            Manipulate = expand_keys(key);
        }
        else {
            Manipulate["default"] = key;
        }
    }
    else {
        Manipulate[key] = desc;
    }
    return Manipulate;
}

varargs mixed CanManipulate(object who, string component){
    mixed val;

    if( !component ){
        component = "default";
    }
    val = Manipulate[component];
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

varargs mixed eventManipulate(object who, string component){
    mixed val;

    if( !component ){
        val = Manipulate["default"];
    }
    else {
        val = Manipulate[component];
    }
    if( arrayp(val) ){
        val = val[query_night()];
    }
    if( stringp(val) ){
        object env;

        env = environment(who);
        who->eventPrint(val);
        if( component ){
            env->eventPrint(who->GetName() + " moves the " + component +
                    " on " + GetDefiniteShort() + ".", who);
        }
        else {
            env->eventPrint(who->GetName() + " moves " +
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

mixed direct_move_obj(object target){
    return CanManipulate(this_player());
}

mixed direct_move_str_on_obj(string str, object target){
    return CanManipulate(this_player(), remove_article(lower_case(str)));
}
