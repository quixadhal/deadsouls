//      /bin/adm/_broadcast.c
//      from the Dead Souls mudlib
//      like shout, except that it overrides ALL blocking
//      created by Descartes of Borg 930823

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {

    if(!archp(previous_object())) return 0;
    message("broadcast",
      sprintf("%%^RED%%^Broadcast message from %s: %%^YELLOW%%^%s%%^RESET%%^",
      (string)this_player()->GetName(), str), users());
    return 1;
}

void help() {
    message("help",
      "Syntax: <broadcast [message]>\n\n"
      "Sends out a message that is in English and immune to all forms of "
      "blocking.  Do not use this for shout matches!", this_player()
    );
}
