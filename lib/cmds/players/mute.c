#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    if( !sizeof(str) )
	return "Syntax: mute <on or off>";
    str = lower_case(str);
    if( str != "on" && str != "off" )
	return "You can only turn mute 'on' or 'off'.";
    this_player()->SetProperty("mute",str == "on");
    message("system",
      "Mute mode turned "+str+".",
      this_player() );
    return 1;
}

void help() {
    message("help",
      "Syntax: mute on\n"
      "        mute off\n\n"
      "This command allows you to avoid or listen to channel messages.\n"
      "See also: gag, env\n\n",
      this_player() );
}
