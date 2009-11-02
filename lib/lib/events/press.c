/*    /lib/events/press.c
 *    from the Dead Souls Mud Library
 *    handles people pressing things
 *    created by Descartes of Borg 960114
 *    Version: @(#) press.c 1.3@(#)
 *    Last modified: 96/12/22
 */

#include <function.h>

static private mapping Press = ([]);

// abstract methods
string GetDefiniteShort();
// end abstract methods

varargs mixed GetPress(string str){
    if( !str ){
        str = "default";
    }
    return Press[str];
}

string array GetPresses(){
    return keys(Press);
}

mapping RemovePress(string item){ 
    map_delete(Press, item); 
    return Press;
}

varargs mapping SetPress(mixed key, mixed desc){
    if( !key ){
        key = "default";
    }
    if( !desc ){
        if( mapp(key) ){
            Press = expand_keys(key);
        }
        else {
            Press["default"] = key;
        }
    }
    else {
        Press[key] = desc;
    }
    return Press;
}

varargs mixed CanPress(object who, string component){
    mixed val;
    if( !component ){
        component = "default";
    }
    val = Press[component];
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

varargs mixed eventPress(object who, string component){
    mixed val;

    if( !component ){
        val = Press["default"];
    }
    else {
        val = Press[component];
    }
    if( arrayp(val) ){
        val = val[query_night()];
    }
    if( stringp(val) ){
        object env;

        env = environment(who);
        who->eventPrint(val);
        if( component ){
            env->eventPrint(who->GetName() + " presses the " + component +
                    " on " + GetDefiniteShort() + ".", who);
        }
        else {
            env->eventPrint(who->GetName() + " presses " +
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

mixed direct_press_obj(object target){
    return CanPress(this_player());
}

mixed direct_press_str_on_obj(string str, object target){
    return CanPress(this_player(), remove_article(lower_case(str)));
}
