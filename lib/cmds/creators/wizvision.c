#include <lib.h>

inherit LIB_DAEMON;

varargs int cmd(string str){
    if(!str){
        write("Wizvision is "+ (this_player()->GetWizVision() ? "on." : "off."));
        return 1;
    }
    if(str == "on"){
        if(this_player()->GetWizVision()){
            write("Your wizvision is already on.\n");
            return 1;
        }
        this_player()->SetWizVision(1);
        if(this_player()->GetWizVision()){
            write("Wizvision is now on.");
        }
        else {
            write("Fail. Wizvision is not on.");
        }
        return 1;
    }
    if(str == "off"){
        if(!(this_player()->GetWizVision())){
            write("Your wizvision is already off.\n");
            return 1;
        }
        this_player()->SetWizVision(0);
        if(!(this_player()->GetWizVision())){
            write("Wizvision is now off.");
        }
        else {
            write("Fail. Wizvision is still on.");
        }
        return 1;
    }
    write("Try: help wizvision");
    return 1;
}

string GetHelp(){
    return ("Syntax: wizvision [ on | off ]\n\n"+
            "Enables clear vision in all lighting situations."); 
}
