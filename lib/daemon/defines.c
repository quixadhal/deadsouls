#include <lib.h>

inherit LIB_DAEMON;
string file, contents, program, header;

static void create() {
    string *includes = ({});
    ::create();
    foreach(string str in get_dir("/secure/include/")){
        if(str == "object.h") continue;
        if(last(str, 2) != ".h") continue;
        includes += ({ "#include <"+str+">" });
    }
    foreach(string str in get_dir("/include/")){
        includes += ({ "#include <"+str+">" });
    } 
    header = implode(includes,"\n")+"\n";
    header += "#ifndef $S\n#define $S \"DEFINED_D: undefined\"\n";
    header += "#endif\n";
    program = "\nmixed defcheck(){"+
        "\nint err;"+
        "\nmixed ret;"+
        "\nerr = catch( ret = $S );"+
        "\nif(err) ret = \"DEFINED_D: error\";"+
        "\nelse if(undefinedp(ret)) ret = \"DEFINED_D: undefined\";"+
        "\nreturn ret;"+
        "\n}";
    write_file("/open/prog.c",header+program,1);
}

string GetDefine(string str){
    mixed tmpret, ret;
    int err;
    if(!str || !stringp(str) || grepp(str, "/") || grepp(str, ".")){ 
        return ret;
    }
    file = DIR_DAEMONS "/tmp/" + str + ".c";
    if( !unguarded((: file_exists($(file)) :)) ) {
        contents = header+program;
        contents = replace_string(contents,"$S",str);
        unguarded((: write_file($(file), contents) :));
    }
    err = catch( tmpret = file->defcheck() );
    if(err || (stringp(tmpret) && !strsrch(tmpret,"DEFINED_D:"))){ 
    }
    else ret = tmpret;
    return ret;
}
