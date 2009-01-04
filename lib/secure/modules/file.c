#include <lib.h>
#include <modules.h>

string array eventReadFunctions(string source);
int eventModString(string file, string param, string replace);

string first_arg, globalstr, globalstr2, globalstr3;
string *global_array;
mixed globalmixed;
string *where_append;

string eventAppendLast(string file, string fun, string addendum){
    int done;
    string *source;
    string junk1, junk2, junk3, junk4, junk5;
    string ret = "";
    globalstr = file;

    if(!grepp(file,"\n") && unguarded( (:file_exists(globalstr):) ) && !check_privs(this_player(),globalstr)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return "";
    }
    if(!grepp(file,"\n") && unguarded( (: file_exists(globalstr):) )) {
        file = unguarded( (: read_file(globalstr) :) );
    }

    unguarded( (: global_array  = this_object()->eventReadFunctions(globalstr) :) );
    source = global_array;
    foreach(string func in source){
        if(sscanf(func,"%s"+fun+"%s(%s)%s\n%s",junk1, junk2, junk3, junk4, junk5) == 5 && !done){
            func = reverse_string(func);
            func = replace_string(func,"}","SIHT_ECALPER",1);
            func = reverse_string(func);
            func = replace_string(func,"REPLACE_THIS",addendum+"\n}");
            done = 1;
}
ret += func + "\n";
}
return ret;
}

string eventAppend(string file, string *params, string addendum){
    int found, count, primary_line, secondary_line;
    string *file_arr;
    string *top_array;
    string *bottom_array;
    string search_str, new_string;
    int tmpnull = 0;

    if(!grepp(file,"\n") && file_exists(file)) globalstr = read_file(file); 
    else globalstr = file;

    if(!grepp(globalstr,"\n") && unguarded( (: file_exists(globalstr):) ) && !check_privs(this_player(),globalstr)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return "";
    }
    if(!grepp(globalstr,"\n") && unguarded( (: file_exists(globalstr):) )) {
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
        if(primary_line > tmpnull){
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
    int numero, count, found, primary_line, secondary_line;
    string *file_arr;
    string *mapping_array;
    string filename, new_string, search_str, mapping_string, junk1, junk2;
    mapping new_mapping = ([]);
    mixed mixed_var;

    if(file_exists(file) && !check_privs(this_player(),file)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return ([]);
    }

    if(!grepp(file,"\n") && file_exists(file)) {
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
    if(destructive) {
        unguarded( (: write_file(globalstr2, globalstr,1) :) );
        unguarded( (: cp(globalstr2, first_arg) :) );
    }
    if(sizeof(new_mapping)) return copy(new_mapping);
    else return ([]);
}

string array eventReadLines(string source){
    if(file_exists(source) && !check_privs(this_player(),source)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return ({});
    }
    globalstr = source;
    unguarded( (: global_array = explode(globalstr,"\n") :) );
    return global_array;
}

string array eventReadFunctions(string source){
    string tmpsource, headers;
    string *ret, *types, *primitives, *beginners, *fun_arr;
    int i, element, infunc;
    mixed line;
    element = -1;
    headers = "";
    line = "";
    fun_arr =  allocate(999);
    primitives = ({"private","static","nomask","varargs"});
    types = ({"int","void","buffer","mapping","mixed","string","array","float"});
    beginners = primitives + types;

    if(grepp(source,"\n") && !file_exists(source)) {
        globalstr2 = source;
        globalstr = generate_tmp(this_player());
        unguarded( (: write_file(globalstr, globalstr2,1) :) );
        source = globalstr;
    }

    tmpsource = generate_tmp(source);

    if(file_exists(source) && !check_privs(this_player(),source)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return ({});
    }

    if(file_exists(tmpsource) && !check_privs(this_player(),tmpsource)){
        write("You do not appear to have write access to this file. Modification aborted.");
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
                        write("You do not appear to have write access to this file. Modification aborted.");
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
                        }

                        else {
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
    int terrain_include = 0;
    if(file_exists(file) && !check_privs(this_player(),file)){
        write("You do not appear to have write access to this file. Modification aborted.");
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

    if(stringp(replace)) { 
        if(globalstr3 == "SetArmorType" || globalstr3 == "SetMealType" || 
                globalstr3 == "SetPosition" || globalstr3 == "SetTerrainType" ||
                globalstr3 == "AddTerrainType" || globalstr3 == "SetSize" ||
                globalstr3 == "SetBodyType" || globalstr3 == "SetRespiration" ||
                globalstr3 == "SetVendorType" || globalstr3 == "SetDamageType"){
            replace = upper_case(replace);
            if(globalstr3 == "SetArmorType") check_include = "/include/armor_types.h";
            if(globalstr3 == "SetVendorType") check_include = "/include/vendor_types.h";
            if(globalstr3 == "SetDamageType") check_include = "/include/damage_types.h";
            if(globalstr3 == "SetMealType") check_include = "/include/meal_types.h";
            if(globalstr3 == "SetPosition") check_include = "/include/position.h";
            if(globalstr3 == "SetTerrainType") check_include = "/include/terrain_types.h";
            if(globalstr3 == "AddTerrainType") check_include = "/include/terrain_types.h";
            if(globalstr3 == "SetSize") check_include = "/include/size_types.h";
            if(globalstr3 == "SetRespiration") check_include = "/include/respiration_types.h";
            if(globalstr3 == "SetBodyType") check_include = "/include/body_types.h";
            if(globalstr3 == "SetSize" && !grepp(replace,"S_")) replace = "S_"+replace; 
            if(globalstr3 == "SetRespiration" && !grepp(replace,"R_")) replace = "R_"+replace;
            if(globalstr3 == "SetBodyType" && !grepp(replace,"B_")) replace = "B_"+replace; 
            if(globalstr3 == "SetVendorType" && !grepp(replace,"VT_")) replace = "VT_"+replace;
            if(globalstr3 == "SetArmorType" && !grepp(replace,"A_")) replace = "A_"+replace;
            if(globalstr3 == "SetTerrainType" && !grepp(replace,"T_")) replace = "T_"+replace;
            if(globalstr3 == "AddTerrainType" && !grepp(replace,"T_")) replace = "T_"+replace;
            if(grepp(replace,"T_")) terrain_include = 1;
            if(globalstr3 == "SetMealType" && !grepp(replace,"MEAL_")) replace = "MEAL_"+replace;
            if(globalstr3 == "SetPosition" && !grepp(replace,"POSITION_")) replace = "POSITION_"+replace;
            if(!grepp(read_file(check_include),replace)) {
                write("Invalid type. Please review "+check_include+" for valid types.");
                return 1;
            }
            if(grepp(replace,"MEAL_ALCOHOL")) replace = "MEAL_DRINK | MEAL_ALCOHOL";
            if(grepp(replace,"MEAL_CAFFEINE")) replace = "MEAL_DRINK | MEAL_CAFFEINE";

            globalmixed = replace;
        }
        else globalmixed = "\""+replace+"\"";
    }
    if(unguarded( (: grepp(read_file(globalstr),globalstr3):) ))
        ret = unguarded( (: replace_matching_line(read_file(globalstr), globalstr3, globalstr3+"("+globalmixed+");") :) );
    else ret = unguarded( (: eventAppend(read_file(globalstr),where_append,"\n"+globalstr3+"("+globalmixed+");\n") :) );
    if(terrain_include && !grepp(ret,"terrain_types.h")) ret = "#include <terrain_types.h>\n" + ret;
    globalstr3 = ret;
    unguarded( (:  write_file(globalstr2, globalstr3, 1) :) );
    globalstr = file;
    unguarded( (: cp(globalstr2, globalstr) :) );
    unguarded( (: rm(globalstr2) :) );
    return 1;
}

int eventModHeader(object ob, string what, string value){
    string newcontents, newline;
    string tmpfile = generate_tmp(ob);
    globalstr = tmpfile;
    globalstr2 = base_name(ob)+".c";

    unguarded( (: cp(globalstr2, globalstr) :) );
    unguarded( (: globalstr3 = read_file(globalstr) :) );

    if(what == "include") {
        what = "#include <";
        if(!grepp(value,".h")) value += ".h>";
        else value += ">";
        newline = "\n"+what + value+"\n";
    }

    if(what == "inherit") {
        value = upper_case(value);
        if(!grepp(value,"LIB_")) value = "LIB_"+value;
        newline = "\n"+what +" "+value+";\n";
    }

    if(grepp(globalstr3,value)) {
        write("That object already contains that line.");
        return 1;
    }
    if(what == "inherit"){
        globalstr3 = this_object()->eventAppend(globalstr3,({"inherit "}),newline);
    }

    else {
        if(grepp(globalstr3,what)){
            globalstr3  =  replace_string(globalstr3,".h>\n",".h>;\n");
            globalstr3  =  replace_string(globalstr3,".h\"\n",".h\";\n");
            unguarded( (: write_file(globalstr,globalstr3,1) :) );

            globalstr3 = this_object()->eventAppend(globalstr3,({"#include "}),newline);
            globalstr3  =  replace_string(globalstr3,".h>;\n",".h>\n");
            globalstr3  =  replace_string(globalstr3,".h\";\n",".h\"\n");

        }
        else {
            globalstr3 = this_object()->eventAppend(globalstr3,({"inherit "}),newline);
        }
    }

    globalstr = generate_tmp(ob);
    unguarded( (: write_file(globalstr,globalstr3,1) :) );

    if( catch(load_object(globalstr))){
        write("This change would hose up the object. Modification aborted.");
        return 1;
    }

    unguarded( (: cp(globalstr, globalstr2) :) );
    reload(ob);
    rm(tmpfile);
    return 1;
}

int eventAddCreate(string file){
    string *contents, *temparray, *temparray2, *beginners;
    string tmpfile;
    int done, add, memnum;

    globalstr = tmpfile;
    globalstr2 = file;

    if(file_exists(file) && !check_privs(this_player(),file)){
        write("You do not appear to have write access to this file. Modification aborted.");
        return 1;
    }

    tmpfile = generate_tmp(file);
    temparray = ({});
    temparray2 = ({});
    contents = unguarded( (: eventReadFunctions(globalstr2) :) );
    beginners = ({"private","static","nomask","varargs"});
    beginners += ({"int","void","buffer","mapping","mixed","string","array","float"});

    foreach(string func in contents){
        if(member_array(func,contents) != 0 && grepp(func,"void create")) memnum = member_array(func,contents);
    }

    if(memnum && memnum != -1){
        if(!grepp(contents[memnum],"::create()")){
            temparray = explode(contents[memnum],"\n");
            foreach(string line in temparray){
                if(line && !starts_with_arr(line, beginners)) add = 1;
                if(add && !done) {
                    temparray2 += ({"::create();"});
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
        }

        else {
            return 2;
        }
    }

    global_array = contents;
    globalstr3 = tmpfile;
    unguarded( (: write_file(globalstr3,implode(global_array,"\n"),1) :) );
    globalstr = tmpfile;
    globalstr2 = file;
    done = unguarded( (: cp(globalstr, globalstr2) :) );
    unguarded( (: rm(globalstr) :) );
    return done;
}



