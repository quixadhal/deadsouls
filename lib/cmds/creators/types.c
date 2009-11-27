#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;
string file, contents, program, header;
string *types = sort_array(keys(TYPES_D->GetTypes()),1);

varargs int cmd(string str){
    int err, i;
    string s1, s2;
    string file = "/open/typecheck.c";
    mixed ret, tmpret;
    if(!str){
        write("Try: help types");
        return 1;
    }
    i = sscanf(str,"%s %s", s1, s2);
    if(i != 2 || member_array(s1, types) == -1){
        write("Try: help types");
        return 1;
    }
    contents = header+program;
    contents = replace_string(contents,"$S",s2);
    unguarded((: write_file($(file), contents,1) :));
    reload(file,0,1);
    err = catch( tmpret = file->typecheck() );
    if(err || (stringp(tmpret) && !strsrch(tmpret,"DEFINED_D:"))){
        write("\n"+ tmpret +" appears to be an invalid integer value.");
        return 1;
    }
    else ret = TYPES_D->eventCalculateTypes(s1, tmpret);
    if(arrayp(ret)) ret = implode(ret," ");
    write("The value "+tmpret+" for "+s1+" is: "+ret);
    return 1;
}

string GetHelp(){
    string type_help = implode(types,", ");
    return ("Syntax: types <name> <value>\n\n"
            "Returns the defined bit checks or defined token "
            "for the specified value.\n"
            "Examples:\n"
            "types parser_error 4\n"
            "types armor_types 512|128\n"
            "types damage_types 128\n\n"
            "Valid type names: "+type_help+".\n\n"
            "Note that entering an invalid value will result in a "
            "rather unattractive error message. This is normal and "
            "harmless.\n"
            "See also: defines");
}

static void create() {
    string *includes = ({});
    ::create();
    foreach(string str in get_dir("/secure/include/")){
        if(str == "object.h") continue;
        includes += ({ "#include <"+str+">" });
    }
    foreach(string str in get_dir("/include/")){
        includes += ({ "#include <"+str+">" });
    } 
    header = implode(includes,"\n");
    program = "\nmixed typecheck(){"+
        "\nint err;"+
        "\nmixed ret;"+
        "\nerr = catch( ret = $S );"+
        "\nif(err) ret = \"DEFINED_D: error\";"+
        "\nelse if(undefinedp(ret)) ret = \"DEFINED_D: undefined\";"+
        "\nreturn ret;"+
        "\n}";
    write_file("/open/prog.c",header+program,1);
}
