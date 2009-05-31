/*    /lib/touch.c
 *    from the Dead Souls Mud Library
 *    for touching things 
 *    created by Descartes of Borg 951008
 *    Version: @(#) touch.c 1.8@(#)
 *    Last modified: 96/12/31
 */

#include <function.h>

static private mixed Touch     = 0;
static private mapping Touches = ([]);

// abstract methods
string GetShort();
// end abstract methods

varargs string GetTouch(string str, object who){
    mixed val;

    if( !str || str == "default" ){
        val = Touch;
    }
    else {
        val = Touches[str];
    }
    if( !val ){
        return 0;
    }
    if( functionp(val) ){
        if( functionp(val) & FP_OWNER_DESTED ){
            return "An error occured in a function pointer.";
        }
        return evaluate(val, who, str);
    }
    else if( arrayp(val) ){
        return val[query_night()];
    }
    else return val;
}

string array GetTouches(){
    return keys(Touches);
}

mapping RemoveTouch(string item){
    if( !item || item == "default" ){
        Touch = 0;
    }
    else {
        map_delete(Touches, item);
    }
    return Touches;
}

varargs mixed SetTouch(mixed array args...){
    if( sizeof(args) == 1 ){
        if( mapp(args[0]) ){
            if( args[0]["default"] ){
                Touch = args[0]["default"];
                map_delete(args[0], "default");
            }
            return (Touches = expand_keys(args[0]));
        }
        else {
            Touch = args[0];
        }
        return args[0];
    }
    else if( sizeof(args) == 2 ){
        if( !args[1] ){
            return SetTouch(args[0]);
        }
        else if( arrayp(args[0]) ){
            foreach(string item in args[0]){
                SetTouch(item, args[1]);
            }
            return args[1];
        }
        else {
            if( !args[0] || args[0] == "default" ){
                Touch = args[1];
                return Touch;
            }
            else {
                Touches[args[0]] = args[1];
                return Touches[args[0]];
            }
        }
    }
    else {
        error("Wrong number of arguments to SetTouch():\n\t"
                "Expected 1 or 2, got " + sizeof(args) + "\n");
    }
}

varargs mixed eventTouch(object who, string str){
    str = GetTouch(str, who);
    if( !str ){
        who->eventPrint("There is nothing to touch.");
        return 1;
    }
    environment(who)->eventPrint(who->GetName() + " touches " + GetShort() +
            ".", who);
    who->eventPrint(str);
    return 1;
}

static void create(){
    Touches = ([]);
}

mixed direct_touch_obj(){
    if( !Touch ){
        return 0;
    }
    else {
        return 1;
    }
}

mixed direct_touch_str_word_obj(string str){
    str = remove_article(lower_case(str));
    if( !Touches[str] ){
        return 0;
    }
    else {
        return 1;
    }
}

