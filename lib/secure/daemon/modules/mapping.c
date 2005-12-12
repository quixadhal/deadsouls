#include <lib.h>
#include <daemons.h>
#include <modules.h>

string func, data, temporary;
int automated;
object target;
mixed newval;

int GetKey(string str);
varargs mapping GetValue(string str, object targ, string tempy, string k1, string v1, mapping MapThing, string passed_fun);

string *key_arr = ({});
mapping NewMap = ([]);

int eventStoreMapping(string savefile, mapping plan){
    string filename = "/tmp/"+savefile;
    if(file_exists(filename) && !check_privs(this_player(),filename)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }
    store_variable("data", plan);
}

varargs int eventStartQuestions(mapping oldmap, object ob, string tmpfile, string auto){
    if(ob) target = ob;
    if(tmpfile) temporary = tmpfile;
    if(auto && auto != "") {
	automated = 1;
	func = auto;
    }

    if(ob && !check_privs(this_player(),ob)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }

    if(file_exists(temporary) && !check_privs(this_player(),temporary)){
	write("You do not appear to have access to this file. Modification aborted.");
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
	write("You do not appear to have access to this file. Modification aborted.");
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
	input_to( (: GetValue :) );
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

varargs mapping GetValue(string str, object targ, string tempy, string k1, string v1, mapping MapThing, string passed_fun){
    int i;
    string repl_key;
    string *mapkeys;
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
	write("You do not appear to have access to this file. Modification aborted.");
	return ([]);
    }
    if(!str || str == "" || str == "." || str == "q") {
	write("Aborting modification. For information on mappings, type: help mappings");
	rm(temporary);
	return ([]);
    }

    if(sscanf(str,"%d",i) == 1) newval = i;
    else newval = str;

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
	if(func) load_object(CREATE_D)->eventResumeMappingChange(target, temporary, NewMap, func);
	else load_object(CREATE_D)->eventResumeMappingChange(target, temporary, NewMap);
    }
    return copy(NewMap);
}

varargs string eventStringifyMap(mapping source, string key_excl, string val_excl){
    string map_str, key, val;

    if(!key_excl) key_excl = ""+time();
    if(!val_excl) val_excl = ""+time();

    map_str = "([\n";

    foreach( key, val in source){
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





