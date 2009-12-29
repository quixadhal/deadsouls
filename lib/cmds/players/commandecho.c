#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string *clrs = ({ "red", "green", "yellow", "blue",
            "cyan", "magenta", "black", "white" });
    if( !sizeof(str) )
        return "Syntax: commandecho <on or off or COLOR>";
    str = lower_case(str);

    if( str != "on" && str != "off" && member_array(str, clrs) == -1 )
        return "Try: help commandecho";
    this_player()->SetProperty("commandecho", ( str == "off" ? 0 : str ));
    message("system",
            "Your commandecho is "+str+".",
            this_player() );
    return 1;
}

string GetHelp() {
    return ("Syntax: commandecho <on | off | COLOR>\n\n"
            "Enables or disables the display of your last command "
            "line after you type it. This is useful only "
            "if you have reprompt and/or charmode enabled. "
            "You can specify a color, using any of the "
            "following: red, green, yellow, blue, cyan, magenta, black, "
            "white. For example:\ncommandecho blue\n"
            "See also: reprompt, charmode");
}

