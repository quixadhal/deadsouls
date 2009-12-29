#include <lib.h>
#include <daemons.h>
#include <modules.h>

string globaltmp, globalstr, globalstr2, func, data, temporary;
int automated;
object target;
mixed newval;

int GetKey(string str);
varargs mapping GetTheValue(string str, object targ, string tempy, string k1, string v1, mapping MapThing, string passed_fun);

string *key_arr = ({});
mapping NewMap = ([]);

int eventStoreMapping(string savefile, mapping plan){
    string filename = "/tmp/"+savefile;
    if(file_exists(filename) && !check_privs(this_player(),filename)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }
    store_variable("data", plan);
}

varargs int eventStartMappingQuestions(mapping oldmap, object ob, string tmpfile, string auto){
    if(ob) target = ob;
    if(tmpfile) temporary = tmpfile;
    if(auto && auto != "") {
        automated = 1;
        func = auto;
    }

    if(ob && !check_privs(this_player(),ob)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }

    if(file_exists(temporary) && !check_privs(this_player(),temporary)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }
    if(sizeof(oldmap)) NewMap = oldmap;
    else NewMap = ([]);
    key_arr = ({});
    write("If you don't understand these questions, type the letter q on a blank "+
            "line and hit enter.\n");
    write("Please enter the first key element for this mapping:\n");
    input_to( (: GetKey :) );
    return 1;
}

int GetKey(string str){
    if(file_exists(temporary) && !check_privs(this_player(),temporary)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }
    if(str && str == "q" ) {
        write("Aborting modification. For information on mappings, type: help mappings");
        rm(temporary);
        return 1;
    }
    if(!str || str == "" || str == "."){
        if(!sizeof(key_arr)){
            write("Aborting modification. For information on mappings, type: help mappings");
            rm(temporary);
            return 1;
        }
        write("Please enter the value for key "+identify(key_arr)+":\n");
        input_to( (: GetTheValue :) );
        return 1;
    }
    else {
        key_arr += ({ str });
        write("Please enter the next key element, or enter a single dot to "
                "finish entering key elements.");
        input_to( (: GetKey :) );
        return 1;
    }
}

varargs mapping GetTheValue(string str, object targ, string tempy, string k1, string v1, mapping MapThing, string passed_fun){
    int i;
    string repl_key;
    string *mapkeys;

    if(!str && !targ && !tempy && !k1 && !v1){
        return 0;
    }
    if(str == "AUTOMATED"){
        NewMap = ([]);
        automated = 1;
        target = targ;
        key_arr = ({ k1 });
        temporary = tempy;
        str = v1;
        if(sizeof(MapThing)) NewMap = MapThing;
        func = passed_fun;
        unguarded( (: cp(base_name(target)+".c",temporary) :) );
    }
    if(file_exists(temporary) && !check_privs(this_player(),temporary)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return ([]);
    }
    if(!str || str == "" || str == "." || str == "q") {
        write("Aborting modification. For information on mappings, type: help mappings");
        rm(temporary);
        return ([]);
    }

    if(sscanf(str,"%d",i) == 1) newval = i;
    else newval = str;

    if(sizeof(replace_string(str, itoa(i), ""))) newval = str;

    mapkeys = keys(NewMap);
    foreach(mixed llave in mapkeys){
        foreach(string nueva_llave in key_arr){
            if(arrayp(llave) && member_array(nueva_llave,llave) != -1) repl_key = llave;
            else if(stringp(llave) && nueva_llave == llave ) repl_key = llave;
        }
    }

    if(repl_key || repl_key != "") map_delete(NewMap, repl_key);
    if(sizeof(key_arr) > 1) NewMap[key_arr] = newval;
    else NewMap[key_arr[0]] = newval;
    if(automated){
        automated = 0;
        if(func) this_object()->eventResumeMappingChange(target, temporary, NewMap, func);
        else this_object()->eventResumeMappingChange(target, temporary, NewMap);
    }
    return copy(NewMap);
}

varargs string eventStringifyMap(mapping source, string key_excl, string val_excl){
    string map_str, key;
    mixed val;

    if(!key_excl) key_excl = ""+time();
    if(!val_excl) val_excl = ""+time();

    map_str = "([\n";

    foreach( key, val in source){
        if(intp(val)){
            map_str += identify(key) + " : " + val +",\n";
        }
        else if(stringp(val) && last(val,1) == "'" && first(val,1) == "'"){
            val = replace_string(val,"'","");
            map_str += identify(key) + " : " + val +",\n";
        }
        else
            map_str += identify(key) + " : " + identify(val) +",\n";
    }
    map_str += "])";
    return map_str;
}



string eventReadThing(string map){
    // This doesn't actually work yet, and perhaps never will,
    // but I'm leaving it here to remind myself to fix it.
    // -Crat

    string new_string;
    int i, iterations;
    string *arr;
    string *new_arr = ({});

    new_string = "";
    arr = explode(map,":");

    foreach(string element in arr){
        string s1, s2, type;
        int num;
        if(!sizeof(new_arr)) new_arr = ({ replace_string(element,"\"", "") });
        else if(sizeof(element) && element != ""){

            if(sscanf(element,"%d ,%s",num,s1) != 2) {
                sscanf(element,"%d,%s",num,s1);
                type = "int";
            }
            if(!s1 || s1 == "") { 
                sscanf(element,"(:%s:)%s", s1, s2);
                type = "function";
            }
            if(!s2 || s2 == "") {
                sscanf(element,"([%s])%s", s1, s2);
                type = "mapping";
            }
            if(!s2 || s2 == "") {
                sscanf(element,"\"%s\"%s", s1, s2);
                type = "string";
            }
            if(!s2 || s2 == "") {
                s1 = element;
                type = "null";
            }

            if(!type || type == ""){
                write("Whoops! An incomprehensible error occurs.");
                return "";
            }

            if(type == "int") new_arr += ({ ""+num });
            else if(type == "function") new_arr += ({ "(: "+s1+" :)" });
            else if(type == "mapping") new_arr += ({ "([ "+s1+" ])" });
            else if(type == "string") new_arr += ({ "\""+s1+"\"" });
            else if(type == "null") new_arr += ({ replace_string(s1,",","",1) });
        }
    }

    iterations = sizeof(new_arr);

    for( i = 0; i < (iterations -2) ; i = i+2  ){
        new_string += new_arr[i]+" : "+new_arr[i+1]+",\n"; 
    }
    new_string += new_arr[iterations -1]+"\n";

    new_string += "])";

    return new_string;
}

int eventSpecialMapHandler(object ob, string func, mixed mode, mixed value){
    string tmp, filename, func2, junk, junk2;
    string *plural_maps;
    int integer;
    mapping NewMap = ([]);
    mapping FirstMap = ([]);
    mapping SecondMap = ([]);

    plural_maps = ({ "SetProperties" });
    if(stringp(value)) value = trim(replace_string(value,mode,""));

    if(stringp(value) && !grepp(lower_case(func),"prop") ){
        if(sscanf(value,"%d",integer) != 1 && 
                sscanf(value,"%d %s",integer,junk) != 2) {
            sscanf(value,"%s %s",tmp,junk);
            if(!tmp || !junk) value = 0;
            else if(sscanf(junk,"%d",integer) != 1) sscanf(junk,"%d %s",integer,junk2);
            if(tmp) mode += " "+tmp;
        } 
        if(integer) value = integer;
        else value = 0;
        if(!intp(value)) value = 0;
    }

    else {
        if(sscanf(value,"%s %s",tmp, junk) == 2){
            mode += " "+tmp;
            value = trim(junk);
        }
    }

    filename = base_name(ob)+".c";
    if(!check_privs(this_player(),filename)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }
    if(!mode || !value){
        write("Invalid entry. Modification cancelled.");
        return 1;
    }

    globalstr = filename;
    unguarded( (: globalstr2 = read_file(globalstr) :) );


    switch(func){
        case "SetProperties" : func2 = "SetProperty";break;
        case "SetSkills" : func2 = "SetSkill";break;
        case "SetStats" : func2 = "SetStat";break;
    }

    FirstMap = this_object()->eventReadPair(filename, func2, 1);
    SecondMap = this_object()->eventReadMapping(filename, ({ func }), 1);
    NewMap = add_maps(FirstMap, SecondMap);
    if(stringp(value) && sscanf(value,"%d",integer) == 1) {
        NewMap[mode] = integer;
    }
    else NewMap[mode] = value;
    globaltmp = generate_tmp(ob);
    unguarded( (: write_file(globaltmp,globalstr2,1) :) );

    if(member_array(func,plural_maps) != -1) 
        this_object()->eventResumeMappingChange(ob,globaltmp,copy(NewMap),func);
    else this_object()->eventAddSettings(ob,globaltmp,copy(NewMap),func2);
    return 1;
}

mapping eventReadPair(string filename, string param, int destructive){
    mixed numerator, denominator;
    int integer;
    string s1, junk, contents, tmp;
    string *file_arr, *line_arr;
    mapping NewMap = ([]);

    if(file_exists(filename) && !check_privs(this_player(),filename)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return ([]);
    }

    globalstr = filename;
    if(file_exists(filename)) {
        unguarded( (: globalstr2 = read_file(globalstr) :) );
    }
    contents = globalstr2;
    file_arr = explode(contents,"\n");
    line_arr = ({});

    foreach(string line in file_arr){
        if(grepp(line,param)) line_arr += ({ line });
    }

    if(!sizeof(line_arr)) return ([]);

    foreach(string element in line_arr){
        element = trim(element);
        if(sscanf(element,param+"(%s);",s1) != -1) sscanf(element,param+"(%s) ;",s1); 
        if(!s1) sscanf(element,param+" (%s);",s1,junk);
        sscanf(s1,"%s,%s",denominator,numerator);
        numerator = trim(numerator);
        denominator = trim(denominator);
        if(sscanf(denominator,"\"%s\"",junk) == 1) denominator = junk;
        if(sscanf(numerator,"%d",integer) == 1) numerator = integer;
        else if(sscanf(numerator,"\"%s\"",junk) == 1) numerator = junk;
        if(stringp(numerator)) numerator = trim(numerator);
        denominator = trim(denominator);
        NewMap[denominator] = numerator;
    }

    if(destructive) {
        globalstr2 = remove_matching_line(globalstr2, param, 1);
        globalstr = generate_tmp();
        unguarded( (: write_file(globalstr,globalstr2,1) :) );
        tmp = globalstr;
        globalstr = filename;
        globalstr2 = tmp;
        unguarded( (: cp(globalstr2, globalstr) :) );
        rm(tmp);
    }
    return copy(NewMap);
}




