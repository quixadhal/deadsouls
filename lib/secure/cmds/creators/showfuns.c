#include <lib.h>

inherit LIB_DAEMON;
mapping FileSize = ([]);
mapping FunctionCache = ([]);
string SaveFile = "/save/showfuns.o";
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
    if(!file_exists(SaveFile)) save_object(SaveFile);
    else restore_object(SaveFile);
}

mixed cmd(string str) {
    string *lines, *raw_lines;
    object ob;
    string tmp, content;

    arr = ({});

    if( !str ) return "You must specify a file.";
    else str = absolute_path((string)this_player()->query_cwd(), str);
    if( !file_exists(str) ) str += ".c";
    if( !file_exists(str) ) return "File " + str + " not found.";
    else if( !(content = read_file(str)) )
	return "Unable to read file " + str + ".";
    else tmp = "";
    content = replace_string(content, " *", " array ");
    //tc("content: "+content);
    if(!sizeof(FunctionCache[str]) || stat(str)[0] != FileSize[str]){
	lines = explode(content, "\n");
	ob = load_object(str);
	if(!ob) return "File cannot be loaded.";
	else arr = query_local_functions(ob);
	raw_lines = filter(lines, (: reverse_memberp($1, types) :) );
	lines = filter(raw_lines, (: reverse_memberp($1, arr) :) );
	foreach(string func in arr){
	    foreach(string element in types){
		foreach(string line in lines){
		    if(regexp(line, element+func)) {
			//tc("line: "+line);
			line = trim(line);
			if(!strsrch(line,"/*") || !strsrch(line,"//") || !strsrch(line,"*")) continue;
			if(!grepp(tmp,func)) tmp += line+"\n";
		    }
		}
	    }
	}
	FileSize[str] = stat(str)[0];
	FunctionCache[str] = tmp;
	save_object(SaveFile);
    }
    //message("system", FunctionCache[str], this_player());
    return FunctionCache[str];
}

int help() {
    message("help", "Syntax: <showfuns [file]>\n\n"
      "Displays the functions defined in the file.",
      this_player());
}
