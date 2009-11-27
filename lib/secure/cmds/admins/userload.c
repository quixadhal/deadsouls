#include <lib.h>
#include <dirs.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string arg){
    int ret;
    object whom;
    if(!archp(previous_object())) return 0;
    if(!arg || arg == "me") arg = this_player()->GetKeyName();
    if(!(whom = find_player(arg))){
        write("The user was not found.");
        return 1;
    }
    whom->CancelCharmode();
    ret = RELOAD_D->ReloadPlayer(arg, 1);
    if(!ret) write("An error occurred.");
    else write("Done.");
    whom = find_player(arg);
    if(whom) whom->CancelCharmode();
    return 1;
}

string GetHelp(){
    return "Syntax: userload <username>\n\n"
        "Reloads the code for users, creates a new body for the specified "
        "user, and switches them into that new body. This command is useful "
        "for testing new functionality in the user object without having "
        "to quit.\n"
        "See also: reload, update, warmboot ";
}
