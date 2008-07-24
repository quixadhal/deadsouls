#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    this_player()->SetPrompt(str);
    this_player()->reset_prompt();
    return 1;
}

void help() {
    message("help",
      "Syntax: prompt <options>"
      "        \n\n"
      "This command allows you to customize your prompt. You may use a static prompt:\n"
      "prompt >\n"
      "Or you may choose special tokens for an active prompt. The tokens available are:\n"
      "$V shows invis status\n"
      "$C shows the command number\n"
      "$H shows maximum health points\n"
      "$h shows current health points\n"
      "$G shows maximum magic points\n"
      "$g shows current magic points\n"
      "$I shows maximum stamina points\n"
      "$i shows current stamina points\n"
      "$P shows current working directory\n"
      "$M shows the mud name\n"
      "$N shows the player's name\n\n"
      "To have your creator character's name and current dir show up:\n"
      "prompt $N $P > \n"
      "In yellow:\n"
      "prompt %%^^YELLOW%%^^$N $P >%%^^RESET%%^^\n"
      "Or for tactical info as a player:\n" 
      "prompt hp:$h/$H mp:$g/$G sp:$i/$I > \n"
      "\n\n",
      this_player() );
}
