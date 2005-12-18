#include <lib.h>
#include <modules.h>

string eventReadFunctions(string source);
int eventModString(string file, string param, string replace);

string first_arg, globalstr, globalstr2, globalstr3;
string *global_array;
mixed globalmixed;

string eventAppend(string file, string *params, string addendum){
    int found, count, primary_line, secondary_line;
    string *file_arr;
    string *top_array;
    string *bottom_array;
    string search_str, new_string;

    globalstr = file;

    if(unguarded( (: file_exists(globalstr):) ) && !check_privs(this_player(),globalstr)){
	write("You do not appear to have access to this file. Modification aborted.");
	return "";
    }


    if(unguarded( (: file_exists(globalstr):) )) {
	file = unguarded( (: read_file(globalstr) :) );
    }

    foreach(string param in params){

	if(!found && param && sizeof(param) && param != "" && stringp(param)){
	    if(strsrch(file,param) != -1){ 
		search_str = param;
		found = 1;
	    }
	}
    }

    if(!found || !search_str || search_str == ""){

	return file;
    }

    file_arr = explode(file,"\n");

    foreach(string line in file_arr){
	if(line && line != "" && strsrch(line, search_str) != -1) {
	    primary_line = member_array(line, file_arr);
	    count = primary_line;
	}
	if(primary_line){
	    count++;
	    if(last(line,1,1) == ";" ) {
		secondary_line = count;
	    }
	}
	if(secondary_line) break;
    }

    top_array = file_arr[0..secondary_line-1];
    bottom_array = file_arr[secondary_line..sizeof(file_arr)-1];

    new_string = implode(top_array,"\n");
    new_string += addendum;
    new_string += implode(bottom_array,"\n");

    return new_string;
}

varargs mapping eventReadMapping(string file, string *params, int destructive){
    int numero, line_range, numm, count, found, primary_line, secondary_line;
    string *file_arr;
    string *mapping_array;
    string new_file, filename, new_string, search_str, mapping_string, junk1, junk2;
    mapping new_mapping = ([]);
    mixed mixed_var;

    if(file_exists(file) && !check_privs(this_player(),file)){
	write("You do not appear to have access to this file. Modification aborted.");
	return ([]);
    }

    if(file_exists(file)) {
	first_arg  =  file;
	file = read_file(file);
    }

    foreach(string param in params){
	if(!found){
	    if(strsrch(file,param) != -1)
		search_str = param;
	    found = 1;
	}
    }

    if(!found){
	return ([]);
    }

    file_arr = explode(file,"\n");

    foreach(string line in file_arr){
	if(strsrch(line, search_str) != -1) {
	    primary_line = member_array(line, file_arr);
	    count = primary_line - 1;
	}
	if(primary_line){
	    count++;
	    if(last(line,1,1 ) == ";") {
		secondary_line = count;
	    }
	}
	if(secondary_line) break;
    }


    if(primary_line != secondary_line ) 
	mapping_array = file_arr[primary_line..secondary_line];
    else mapping_array = ({ file_arr[primary_line] });

    mapping_array = file_arr[primary_line..secondary_line];

    new_string = implode(mapping_array," ");
    if(!new_string) write("Problem here.");
    if(sscanf(new_string,"%s([%s])%s",junk1,mapping_string, junk2) < 3){
	write("It's a null mapping");
	return ([]);
    }
    //tc("map: "+mapping_string);
    mapping_array = explode(mapping_string,",");
    foreach(string foo in mapping_array){
	string *sub_array;
	if(!foo) break;
	if(strsrch(foo,":") == -1) break;
	sub_array = explode(foo,":");
	if(sub_array[0] && sub_array[1]) {
	    sub_array[0] = trim(replace_string(replace_string(sub_array[0]," \t",""),"\"",""));
	    sub_array[1] = trim(replace_string(replace_string(sub_array[1]," \t",""),"\"",""));
	    if(sscanf(sub_array[1],"%d",numero) == 1) mixed_var = numero;
	    else  mixed_var = sub_array[1];
	    new_mapping[sub_array[0]] = mixed_var;
	}
    }

    if(!first_arg) first_arg = "NULL";

    found = 0;

    globalstr3 = search_str;
    unguarded( (: globalstr = remove_matching_line(read_file(first_arg),globalstr3,1) :) );
    globalstr2 = generate_tmp(file);
    //write("globalstr2: "+globalstr2);
    if(destructive) {
	//write("first arg: "+first_arg);
	//unguarded( (: write_file("/realms/"+this_player()->GetKeyName()+"/tmp/mapping_destructive."+this_player()->GetKeyName(),globalstr,1) :) );
	//unguarded( (: cp("/realms/"+this_player()->GetKeyName()+"/tmp/mapping_destructive."+this_player()->GetKeyName(),first_arg) :) );
	unguarded( (: write_file(globalstr2, globalstr,1) :) );
	unguarded( (: cp(globalstr2, first_arg) :) );
	//unguarded( (: tc("read_file(globalstr2): "+read_file(globalstr2)) :) );
	//unguarded( (: write("read_file(first_arg): "+read_file(first_arg)) :) );
    }
    if(sizeof(new_mapping)) return copy(new_mapping);
    else return ([]);
}

string array eventReadFunctions(string source){
    string tmpsource, fun_str, new_file, headers;
    string *ret, *types, *primitives, *beginners, *fun_arr;
    int kickoff, i, element, infunc;
    mixed line;
    element = -1;
    headers = "";
    line = "";
    fun_arr =  allocate(999);
    primitives = ({"private","static","nomask","varargs"});
    types = ({"int","void","buffer","mapping","mixed","string","array","float"});
    beginners = primitives + types;

    tmpsource = generate_tmp(source);

    if(file_exists(source) && !check_privs(this_player(),source)){
	write("You do not appear to have access to this file. Modification aborted.");
	return ({});
    }

    if(file_exists(tmpsource) && !check_privs(this_player(),tmpsource)){
	write("You do not appear to have access to this file. Modification aborted.");
	return ({});
    }
    globalstr3 = tmpsource;
    globalstr = source;
    unguarded( (: cp(globalstr, globalstr3) :) );
    if(!file_exists(source)) return ({"Source read failed."});
    if(!file_exists(tmpsource)) return ({"Read failed."});


    for(i=1; line; i++){
	if(!line = read_file(source, i, 1)) break;
	if(!line || !stringp(line) || line == "") break;


	if(line && !infunc && !starts_with_arr(line, beginners)) headers += line;



	else if(line && !infunc && starts_with_arr(line, beginners) ){

	    element++;
	    infunc = 1;
	    if(!sizeof(fun_arr[element])) fun_arr[element] = line;
	    else fun_arr[element] += line;
	}

	else if(line && infunc && !starts_with_arr(line, beginners) ){

	    fun_arr[element] += line;
	}

	else if(line && infunc && starts_with_arr(line, beginners) && !grepp(line,"(")){
	    fun_arr[element] += line;
	}


	else if(line && infunc && starts_with_arr(line, beginners) && grepp(line,"(")){
	    infunc = 0;
	    i--;
	}
    }

    ret = ({headers});
    foreach(string item in fun_arr){
	if(item && sizeof(item)) ret += ({newline_trim(item)});
    }
    rm(tmpsource);
    return ret;
}

int eventAddInit(string file){
    string *contents, *temparray, *temparray2, *beginners;
    string tmpfile;
    int done, add, memnum;

    globalstr = tmpfile;
    globalstr2 = file;

    if(file_exists(file) && !check_privs(this_player(),file)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }

    tmpfile = generate_tmp(file);
    temparray = ({});
    temparray2 = ({});
    contents = unguarded( (: eventReadFunctions(globalstr2) :) );
    beginners = ({"private","static","nomask","varargs"});
    beginners += ({"int","void","buffer","mapping","mixed","string","array","float"});

    foreach(string func in contents){
	if(member_array(func,contents) != 0 && grepp(func,"void init")) memnum = member_array(func,contents);
    }

    if(memnum && memnum != -1){
	if(!grepp(contents[memnum],"::init()")){
	    temparray = explode(contents[memnum],"\n");
	    foreach(string line in temparray){
		if(line && !starts_with_arr(line, beginners)) add = 1;
		if(add && !done) {
		    temparray2 += ({"::init();"});
		    temparray2 += ({line});
		    done = 1;
		}
		else {
		    temparray2 += ({line});
		}
	    }
	    contents[memnum] = implode(temparray2,"\n");
	    global_array = contents;
	    globalstr3 = tmpfile;
	    unguarded( (: write_file(globalstr3,implode(global_array,"\n"),1) :) );
	    //write_file(tmpfile,implode(contents,""));
	}

	else {
	    //write("File already contains a working init function.");
	    return 2;
	}
    }

    else {
	contents += ({ "void init(){\n::init();\n}" });
	global_array = contents;
	globalstr3 = tmpfile;
	unguarded( (: write_file(globalstr3,implode(global_array,"\n"),1) :) );
    }
    globalstr = tmpfile;
    globalstr2 = file;
    done = unguarded( (: cp(globalstr, globalstr2) :) );
    unguarded( (: rm(globalstr) :) );
    return done;
}

varargs int eventModString(string file, string param, mixed replace, string *params){
    string check_include, ret, tmpfile;
    string *where_append;
    if(file_exists(file) && !check_privs(this_player(),file)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }
    if(!params) where_append = ({"SetLong","SetShort"});
    if(params && !sizeof(params)) where_append = ({"SetLong","SetShort"});
    else where_append = params;

    tmpfile = generate_tmp(file);
    globalstr = file;
    globalstr2 = tmpfile;
    globalstr3 = param;
    globalmixed = replace;
    //write("grepp(read_file(file),globalstr3): "+ unguarded( (: grepp(read_file(globalstr),globalstr3) :) ));

    if(stringp(replace)) { 
	if(globalstr3 == "SetArmorType" || 
	  globalstr3 == "SetVendorType" || globalstr3 == "SetDamageType"){
	    replace = upper_case(replace);
	    if(globalstr3 == "SetArmorType") check_include = "/include/armor_types.h";
	    if(globalstr3 == "SetVendorType") check_include = "/include/vendor_types.h";
	    if(globalstr3 == "SetDamageType") check_include = "/include/damage_types.h";
	    if(globalstr3 == "SetVendorType" && !grepp(replace,"VT_")) replace = "VT_"+replace;
	    if(globalstr3 == "SetArmorType" && !grepp(replace,"A_")) replace = "A_"+replace;
	    if(!grepp(read_file(check_include),replace)) {
		write("Invalid type. Please review "+check_include+" for valid types.");
		return 1;
	    }
	    globalmixed = replace;
	}
	else globalmixed = "\""+replace+"\"";
    }
    if(unguarded( (: grepp(read_file(globalstr),globalstr3):) ))
	ret = unguarded( (: replace_matching_line(read_file(globalstr), globalstr3, globalstr3+"("+globalmixed+");") :) );
    else ret = eventAppend(file,where_append,"\n"+globalstr3+"("+globalmixed+");\n");
    ret = replace_line(ret,({"customdefs.h"}), "#include \""+homedir(this_player())+"/customdefs.h\"");
    globalstr3 = ret;
    unguarded( (:  write_file(globalstr2, globalstr3, 1) :) );
    //tc("ret: "+ret);
    //tc(tmpfile+": "+read_file(tmpfile));
    unguarded( (: cp(globalstr2, globalstr) :) );
    unguarded( (: rm(globalstr2) :) );
    return 1;
}
