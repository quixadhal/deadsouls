#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;
string *arr = ({});
string *types = ({ "void ", "status ",
        "string ", "string \*", "string array ",
        "int ", "int *", "int array ",
        "object ", "object *", "object array ",
        "mapping ", "mapping *", "mapping array ",
        "mixed ", "mixed *", "mixed array ",
        "float ", "float *", "float array ",
        "function ", "function *", "function array " });

void create(){
    daemon::create();
}

mixed cmd(string str) {
    string *lines, *raw_lines;
    object ob;
    string tmp, content;
    mixed *fs;
    arr = ({});

    if( !str ) return "You must specify a file.";
    else if(this_player()) str = absolute_path(this_player()->query_cwd(), str);
    if( !file_exists(str) ) str += ".c";
    if( !file_exists(str) ) return "File " + str + " not found.";
    else if( !(content = read_file(str)) )
        return "Unable to read file " + str + ".";
    else tmp = "";
    content = replace_string(content, " *", " array ");
    if(!tmp = FUNCTION_D->GetFunctions(str)){
        tmp = "";
        lines = explode(content, "\n");
        ob = load_object(str);
        fs = functions(ob);
        if(!ob) return "File cannot be loaded.";
        else arr = query_local_functions(ob);
        raw_lines = filter(lines, (: reverse_memberp($1, types) :) );
        lines = filter(raw_lines, (: reverse_memberp($1, arr) :) );
        foreach(string func in arr){
            foreach(string element in types){
                foreach(string line in lines){
                    if(regexp(line, element+func)) {
                        line = trim(line);
                        if(!strsrch(line,"/*") || !strsrch(line,"//") || !strsrch(line,"*")) continue;
                        if(last(line,1) == ";") continue;
                        if(!grepp(tmp,func)) tmp += line+"\n";
                    }
                }
            }
        }
        FUNCTION_D->ReceiveFunctionData(str, tmp, stat(str)[0], fs);
    }
    return tmp;
}

string GetHelp(){
    return ("Syntax: showfuns <file>\n\n"
            "Displays the functions defined in the file.");
}
