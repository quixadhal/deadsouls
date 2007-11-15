#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    string *raw_array;
    string *grps = groups();
    string ret = "";
    PLAYERS_D->CompileCreList();
    raw_array = sort_array(PLAYERS_D->GetCreatorList(),1);
    foreach(string wiz in raw_array){
        string tmp = wiz;
        foreach(string group in grps){
            if(member_group(wiz, group)) tmp += " "+group+",";
        }
        if(last(tmp,1) == ",") tmp = truncate(tmp,1);
        ret += capitalize(tmp)+"\n";
    }
    print_long_string(this_player(),"Known active creators: \n"+ret);
    return 1;
}

string GetHelp(string str) {
    return "Syntax: wizlist\n"
    "Displays a list of known active (non-ridded) creators.\n "
    "";
}
