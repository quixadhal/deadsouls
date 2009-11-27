#include <lib.h>

inherit LIB_DAEMON;

varargs int cmd(string str){
    if(!str){
        write("Annoyblock is "+ (this_player()->GetAnnoyblock() ? "on." : "off."));
        return 1;
    }
    if(str == "on"){
        if(this_player()->GetAnnoyblock()){
            write("Your annoyblock is already on.\n");
            return 1;
        }
        this_player()->SetAnnoyblock(1);
        if(this_player()->GetAnnoyblock()){
            write("Annoyblock is now on.");
        }
        else {
            write("Fail. Annoyblock is not on.");
        }
        return 1;
    }
    if(str == "off"){
        if(!(this_player()->GetAnnoyblock())){
            write("Your annoyblock is already off.\n");
            return 1;
        }
        this_player()->SetAnnoyblock(0);
        if(!(this_player()->GetAnnoyblock())){
            write("Annoyblock is now off.");
        }
        else {
            write("Fail. Annoyblock is still on.");
        }
        return 1;
    }
    write("Try: help annoyblock");
    return 1;
}

string GetHelp(){
    return ("Syntax: annoyblock [ on | off ]\n\n"
            "Blocks annoying messages like \"Joe looks around.\""); 
}
