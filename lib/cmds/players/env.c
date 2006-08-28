#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    write("Screen: \t\t"+identify(this_player()->GetScreen()));
    write("Terminal: \t\t"+this_player()->GetTerminal());
    write("Brief mode: \t\t"+ ( (this_player()->GetBriefMode()) ? "on" : "off" ));
    write("Playerkiller mode: \t"+ ( (this_player()->GetPK()) ? "on" : "off" ));
    if(creatorp(this_player())) 
	write("Debug mode: \t\t"+ ( (this_player()->GetProperty("debug")) ? "on" : "off" ));
    return 1;
}

void help() {
    message("help", "Syntax: <env>\n\n"
      "Displays your basic interface and play settings. " +
      "See also: brief, terminal, screen", this_player());
}
