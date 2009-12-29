#include <lib.h>

inherit LIB_DAEMON;

varargs int cmd(string str){
    if(!str){
        write("Godmode is "+ (this_player()->GetGodMode() ? "on." : "off."));
        return 1;
    }
    if(str == "on"){
        if(this_player()->GetGodMode()){
            write("You are already in godmode.\n");
            return 1;
        }
        this_player()->SetGodMode(1);
        if(this_player()->GetGodMode()){
            write("Godmode is now on.");
        }
        else {
            write("Fail. Godmode is not on.");
        }
        return 1;
    }
    if(str == "off"){
        if(!(this_player()->GetGodMode())){
            write("You are already not in godmode.\n");
            return 1;
        }
        this_player()->SetGodMode(0);
        if(!(this_player()->GetGodMode())){
            write("Godmode is now off.");
        }
        else {
            write("Fail. Godmode is still on.");
        }
        return 1;
    }

    write("Try: help godmode");
    return 1;
}

string GetHelp(){
    return ("Syntax: godmode [ on | off ]\n\n"
            "Enables or disables immunity to most forms of lethal damage."); 
}
