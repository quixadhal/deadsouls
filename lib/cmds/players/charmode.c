#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
#ifndef __GET_CHAR_IS_BUFFERED__
    write("This game does not support this mode. Ask your "+
            "mud administrator to consider recompiling with "+
            "GET_CHAR_IS_BUFFERED defined.");          
    return 1;
#endif
    if( !sizeof(str) )
        return "Syntax: charmode <on or off>";
    str = lower_case(str);

    if( str != "on" && str != "off" )
        return "You can only turn charmode 'on' or 'off'";
    if(str == "on") this_player()->SetCharmode(1);
    else this_player()->CancelCharmode();
    this_player()->SetProperty("reprompt", this_player()->GetCharmode());
    message("system",
            "Your charmode mode is turned "+str+".",
            this_player() );
    return 1;
}

string GetHelp() {
    return ("Syntax: charmode [on | off]\n\n"
            "Enables or disables 'character mode' for your terminal. "
            "Unless you are using a very basic mud client (such as "
        "a Linux telnet command) you should not enable this, since "
            "most mud clients prefer to use line mode and will break "
            "if character mode is enabled.\n"
            "See also: reprompt, prompt, env");
}

