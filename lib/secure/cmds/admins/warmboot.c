#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string arg){
    if(!archp(previous_object())) return 0;
    RELOAD_D->WarmBoot();
    write("Warm boot requested.");
    return 1;
}

string GetHelp(){
    return "Syntax: warmboot\n\n"
        "Reloads and updates objects and users in memory such "
        "that changes to code come into effect, as if the mud "
        "had been rebooted. This command does not disconnect players. "
        "WARNING: People in the process of editing files will lose "
        "unsaved changes to the files being edited.\n"
        "See also: reload, update, userload ";
}
