#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;
string funcname;
string *sourcefiles = ({});

string eventFindInstances(){
    string *raw_list = load_object("/secure/cmds/admins/files")->cmd("-c "+"/lib/");
    string cooked_list = "";
    sourcefiles = ({});
    foreach(string file in raw_list){
	string funex;
	if(last(file, 2) != ".c") continue;
	if(grepp(load_object("/secure/cmds/creators/showfuns")->cmd(file), funcname)){
	    funex = function_exists(funcname,load_object(file),1);
	    if(funex && !grepp(cooked_list,funex+"\n")){
		sourcefiles += ({ last_string_element(funex,"/") });
		cooked_list += funex+"\n";
	    }
	}
    }
    return cooked_list;
}

mixed cmd(string args) {
    string ret;

    if(!args || args == ""){
	write("You'll need to be more specific. Try: help findfun");
	return 1;
    }
    funcname = args;
    ret = eventFindInstances();
    write(ret);
    return 1;
}

void help() {
    message("help", "Syntax: <findfun FUNCTION_NAME>\n\n"
      "Tells you which lib file a function is defined in.\n"
      "Example: findfun AddHP\n"
      "\n\n"
      "See also: doctool, showfuns", this_player());
}
