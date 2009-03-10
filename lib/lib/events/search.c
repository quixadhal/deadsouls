/*    /lib/search.c
 *    from the Dead Souls Mud Library
 *    allows people to search things
 *    created by Descartes of Borg 951008
 *    Version: @(#) search.c 1.7@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>
#include <function.h>

static private mixed Search     = 0;
static private mapping Searches = ([]);

// abstract methods
string GetShort();
// end abstract methods

mapping GetTraps(){
    mapping shads = this_object()->GetShadows();
    foreach(mixed key, mixed val in shads){
        if(!inherits(LIB_BOOBYTRAP_SHADOW, key)) map_delete(shads, key);
        else shads[key] = ([ "level" : key->GetTrapLevel(), "description" : key->GetTrapDescription() ]);
    }
    return shads;
}

mapping FoundTraps(){
    mapping Traps = GetTraps();
    int detection_level = 0;
    if(sizeof(Traps)){
        if(this_player()->GetSkill("detection"))
            detection_level += this_player()->GetSkill("detection")["level"];
        detection_level += this_player()->GetStat("luck")["level"];
        detection_level += this_player()->GetStat("intelligence")["level"];
        foreach(mixed key, mixed val2 in Traps){
            if(detection_level < val2["level"]) map_delete(Traps, key);
        }
    }
    return Traps;
}

varargs string GetSearch(string str, object who){
    mixed val;
    mapping Traps = FoundTraps();
    string trapdesc = "You discover it is boobytrapped!\n";

    if(sizeof(Traps)){
        int i = 0;
        foreach(mixed key, mixed val2 in Traps){
            i++;
            trapdesc += i+ordinal(i)+" trap: "+val2["description"] + "\n";
        }
        if(i == 1) trapdesc = replace_string(trapdesc,"1st trap: ","");
    }

    if(!sizeof(Traps)) trapdesc = "";

    if( !str || str == "default" ){
        val = Search;
    }
    else {
        val = Searches[str];
    }
    if(val && stringp(val) && sizeof(Traps)){
        val += "\n"+ trapdesc;
    }
    if(!val && sizeof(Traps)){
        val = trapdesc;
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
        return val[query_night()]+trapdesc;
    }
    else return val;
}

string array GetSearches(){
    return keys(Searches);
}

mapping RemoveSearch(string item){
    if( !item || item == "default" ){
        Search = 0;
    }
    else {
        //map_delete(Searches, item);
        Searches["item"] = "You find nothing.";
    }
    return Searches;
}

varargs mixed SetSearch(mixed array args...){
    if( sizeof(args) == 1 ){
        if( mapp(args[0]) ){
            if( args[0]["default"] ){
                Search = args[0]["default"];
                map_delete(args[0], "default");
            }
            return (Searches = expand_keys(args[0]));
        }
        else {
            Search = args[0];
        }
        return args[0];
    }
    else if( sizeof(args) == 2 ){
        if( !args[1] ){
            return SetSearch(args[0]);
        }
        else if( arrayp(args[0]) ){
            foreach(string item in args[0]){
                SetSearch(item, args[1]);
            }
            return args[1];
        }
        else {
            if( !args[0] || args[0] == "default" ){
                Search = args[1];
                return Search;
            }
            else {
                Searches[args[0]] = args[1];
                return Searches[args[0]];
            }
        }
    }
    else {
        error("Wrong number of arguments to SetSearch():\n\t"
                "Expected 1 or 2, got " + sizeof(args) + "\n");
    }
}

varargs mixed eventSearch(object who, string str){
    str = GetSearch(str, who);
    if( !str ){
        who->eventPrint("You find nothing.");
        return 1;
    }
    environment(who)->eventPrint(who->GetName() + " searches "
            "for something.", who);
    who->eventPrint(str);
    return 1;
}

mixed direct_search_obj(){
    if( !Search && !sizeof(FoundTraps()) ){
        return 0;
    }
    else {
        return 1;
    }
}

mixed direct_search_str_word_obj(string str){
    str = remove_article(lower_case(str));
    if( !Searches[str] || !sizeof(GetTraps()) ){
        return 0;
    }
    else {
        return 1;
    }
}
