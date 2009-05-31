#include <function.h>

static private mapping Scratch = ([]);

// abstract methods
string GetDefiniteShort();
// end abstract methods

varargs mixed GetScratch(string str){
    if( !str ){
        str = "default";
    }
    return Scratch[str];
}

string array GetScratches(){
    return keys(Scratch);
}

mapping RemoveScratch(string item){ 
    map_delete(Scratch, item); 
    return Scratch;
}

varargs mapping SetScratch(mixed key, mixed desc){
    if( !key ){
        key = "default";
    }
    if( !desc ){
        if( mapp(key) ){
            Scratch = expand_keys(key);
        }
        else {
            Scratch["default"] = key;
        }
    }
    else {
        Scratch[key] = desc;
    }
    return Scratch;
}

varargs mixed CanScratch(object who, string component){
    return 1;
}

varargs mixed eventScratch(object who, mixed component){
    mixed val;

    if( !component ){
        val = Scratch["default"];
    }
    else {
        val = Scratch[component];
    }
    if( arrayp(val) ){
        val = val[query_night()];
    }
    if( stringp(val) ){
        object env;

        env = environment(who);
        who->eventPrint(val);
        if( component ){
            env->eventPrint(who->GetName() + " scratches on the " + component +
                    " on " + GetDefiniteShort() + ".", who);
        }
        else {
            env->eventPrint(who->GetName() + " scratches on " +
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

mixed direct_scratch_obj(object target){
    return CanScratch(this_player());
}

mixed direct_scratch_on_obj(object target){
    return direct_scratch_obj(target);;
}

mixed direct_scratch_str_on_obj(string str, object target){
    return CanScratch(this_player(), remove_article(lower_case(str)));
}

mixed direct_scratch_on_str_on_obj(string str, object target){
    return direct_scratch_str_on_obj(str,target);
}

