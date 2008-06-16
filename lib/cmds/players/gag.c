#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    if( !sizeof(str) )
        return "Syntax: gag <on or off>";
    str = lower_case(str);
    if( str != "on" && str != "off" )
        return "You can only turn gag 'on' or 'off'.";
    this_player()->SetProperty("gag",str == "on");
    message("system",
      "Gag mode turned "+str+".",
      this_player() );
    return 1;
}

void help() {
    message("help",
      "Syntax: gag on\n"
      "        gag off\n\n"
      "This command prevents you from talking on channels. Good for when you're hiding.\n"
      "See also: mute, env\n\n",
      this_player() );
}
