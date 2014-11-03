/* Cygnus@Dead Souls */

#include <lib.h>

inherit LIB_DAEMON;

int cmd(){
    int i;
    object *user_ob;
    string ret, name, *user_name;

    user_ob = users();
    user_name = ({ });
    for (i = 0; i < sizeof(user_ob); i++) {
        if(user_ob[i]){
            if( user_ob[i]->GetInvis() )
                continue;
            name = user_ob[i]->GetKeyName();
            if (stringp(name))
                user_name += ({ capitalize(name) });
        }
    }
    user_name = sort_array(user_name, "sort_names");
    ret = format_page(user_name, 4);
    ret += sprintf("Total : %d", sizeof(user_name));
    if(sizeof(ret) < __LARGEST_PRINTABLE_STRING__ ) write(ret);
    else this_player()->eventPage(ret);
    return 1;
}

int sort_names(string name1, string name2){
    if (name1 == name2)
        return 0;
    if (name1 < name2)
        return -1;
    else
        return 1;
}

string GetHelp(){
    return "Syntax: users\n\n"
        "Lists the names of player logged in. "
        "A shorter and quicker version of \"who\".\n"
        "See also: who, where\n"
        "See also: say, tell, class\n";
}
