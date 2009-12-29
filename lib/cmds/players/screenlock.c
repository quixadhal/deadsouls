#include <lib.h>

inherit LIB_DAEMON;

    mixed cmd(string str) {
        if( !sizeof(str) )
            return "Syntax: screenlock <on or off>";
        str = lower_case(str);
        if( str != "on" && str != "off" )
            return "You can only turn screenlock 'on' or 'off'";
        this_player()->SetProperty("screenlock", (str == "on"));
        message("system",
                "Screenlock mode turned "+str+".",
                this_player() );
        return 1;
    }

string GetHelp(){
    return ("Syntax: screenlock [on | off]\n\n"
            "This command allows you to toggle whether your client is "
            "allowed to automatically change your screen size settings "
            "after the initial login negotiation.\n"
            "See also: screen, terminal, env");
}
