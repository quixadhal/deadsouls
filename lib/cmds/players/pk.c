#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    if(!sizeof(args) || (args != "on" && args != "off") ){
        write("Syntax: pk [ on | off ]");
        return 1;
    }
    if(args == "off"){
        if(creatorp(this_player())){
            if(!(this_player()->GetPK())) write("Your PK flag is already off.");
            else write("Your PK flag is now off.");
            this_player()->SetPK(0);
            return 1;
        }
        else {
            write("You must ask a creator to disable your PK flag.");
            return 1;
        }
    }
    if((this_player()->GetPK())){
        write("You are already a player killer.");
        return 1;
    }
    if(this_player()->GetProperty("PK warned")){
        this_player()->SetProperty("PK warned",0);
        write("You are now a player killer.");
        this_player()->SetPK(1);
        return 1;
    }
    else {
        write("If you enable your PK flag, only a creator can disable it.");
        write("If you're 100% sure you want to PK, then issue the command again.");
        this_player()->SetProperty("PK warned",1);
        return 1;
    }
}

string GetHelp() {
    return ("Syntax: pk [ on | off ]\n\n"
            "\tUnless you are set as a player killer, you cannot engage " +
            "in player versus player combat. This command toggles your " +
            "player killer setting. Once they are set as PK, players cannot "+
            "unset it without the intervention of a creator.\n"
            "\tPlayer killing also requires the mud to have its global PK "+
            "setting enabled in its config file, and the room you are in "+
            "must be PK enabled as well.\n"
            "See also: env");
}
