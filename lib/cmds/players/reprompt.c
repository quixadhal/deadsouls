#include <lib.h>

inherit LIB_DAEMON;

    mixed cmd(string str) {
        if( !sizeof(str) )
            return "Syntax: reprompt <on or off>";
        str = lower_case(str);
        if( str != "on" && str != "off" )
            return "You can only turn reprompt 'on' or 'off'";
        this_player()->SetProperty("reprompt", ( str == "on" ? 1 : 0 ));
        message("system",
                "Reprompt mode turned "+str+".",
                this_player() );
        return 1;
    }

string GetHelp() {
    return ("Syntax: reprompt [on | off]\n\n"
            "Enables or disables the redrawing of your prompt after "
            "receiving messages. Not all clients support this. Note "
            "that half-issued commands may wind up looking strange "
            "unless charmode is also enabled.\n"
            "See also: charmode, prompt, env");
}

