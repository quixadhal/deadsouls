#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str){
    if(!sizeof(str)){
        this_player()->SetUserPath(0);
    }
    else {
        if(!directory_exists(str)){
            write("That path appears inaccessible. No change made.");
        }
        else {
            if(last(str, 1) != "/") str += "/";
            this_player()->SetUserPath(str);
        }
    }
    write("Your current home directory is: "+user_path(this_player()));
    return 1;
}

string GetHelp() {
    return ("Syntax: homedir [directory]\n\n"
            "Sets the specified directory to be your \"home\" "
            "directory, i.e. the directory your path changes to "
            "when you type \"cd\" with no arguments. If you do "
            "not specify a directory, your home directory is "
            "reset to your default.\nNOTE: Changing your home "
            "directory may have an impact on where the QCS puts items "
            "you create.\n"
            "See also: pwd, cd, env, homeroom");
}
