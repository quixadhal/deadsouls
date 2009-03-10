/*    /lib/events/smell.c
 *    From the Dead Souls Mud Library
 *    Handles items with smells
 *    Created by Descartes of Borg 951008
 *    Version: @(#) smell.c 1.7@(#)
 *    Last Modified: 96/12/31
 */

#include <function.h>

static private mixed Smell    = 0;
static private mapping Smells = ([]);

// abstract methods
string GetShort();
// end abstract methods

varargs string GetSmell(string str, object who){
    mixed val;

    if( !str || str == "default" ){
        val = Smell;
    }
    else {
        val = Smells[str];
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

string array GetSmells(){
    return keys(Smells);
}

mapping RemoveSmell(string item){
    if( !item || item == "default" ){
        Smell = 0;
    }
    else {
        map_delete(Smells, item);
    }
    return Smells;
}

/**
 * SetSmell(string description)
 * SetSmell(function call_for_description)
 * SetSmell(string array day_and_night_descriptions)
 * SetSmell(mapping thing_desc_pairs)
 * SetSmell(string thing, string|function|string array description)
 */
varargs mixed SetSmell(mixed array args...){
    if( sizeof(args) == 1 ){
        if( mapp(args[0]) ){
            if( args[0]["default"] ){
                Smell = args[0]["default"];
                map_delete(args[0], "default");
            }
            return (Smells = expand_keys(args[0]));
        }
        else {
            Smell = args[0];
        }
        return args[0];
    }
    else if( sizeof(args) == 2 ){
        if( !args[1] ){
            return SetSmell(args[0]);
        }
        else if( arrayp(args[0]) ){
            foreach(string item in args[0]){
                SetSmell(item, args[1]);
            }
            return args[1];
        }
        else {
            if( !args[0] || args[0] == "default" ){
                Smell = args[1];
                return Smell;
            }
            else {
                Smells[args[0]] = args[1];
                return Smells[args[0]];
            }
        }
    }
    else {
        error("Wrong number of arguments to SetSmell():\n\t"
                "Expected 1 or 2, got " + sizeof(args) + "\n");
    }
}

varargs mixed eventSmell(object who, string str){
    str = GetSmell(str, who);
    if( !str ){
        who->eventPrint("There is nothing to smell.");
        return 1;
    }
    environment(who)->eventPrint(who->GetName() + " smells " + GetShort() +
            ".", who);
    who->eventPrint(str);
    return 1;
}

mixed direct_smell_obj(){
    if( !Smell ){
        return "You notice no unusual odor.";
    }
    else {
        return 1;
    }
}

mixed direct_smell_str_word_obj(string str){
    str = remove_article(lower_case(str));
    if( !Smells[str] ){
        return "You notice no unusual odor.";
    }
    else {
        return 1;
    }
}

mapping GetSmellMap(){
    mapping Smells = ([]);
    foreach(object ob in this_object()->GetDummyItems()){
        if( ob->GetSmell() ){
            Smells[ob->GetId()] = ob->GetSmell();
        }
    }
    if(this_object()->GetSmell()) Smells["default"] = this_object()->GetSmell();
    return copy(Smells);

}

