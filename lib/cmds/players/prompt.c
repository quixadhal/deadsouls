#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    if(!sizeof(str)){
        write(this_player()->GetPromptString());
        return 1;
    }
    this_player()->SetPrompt(str);
    this_player()->reset_prompt();
    return 1;
}

string GetHelp() {
    string cre = "$P shows current working directory\n"+
        "To have your creator character's name "+
        "and current dir show up:\nprompt $N $P >\nIn yellow:\n"+
        "prompt %%^^YELLOW%%^^$N $P >%%^^RESET%%^^";
    if(!creatorp(this_player())) cre = "";
    return ("Syntax: prompt [options]\n\n"
            "This command allows you to customize your prompt. You may use a static prompt:\n"
            "prompt >\n"
            "Or you may choose special tokens for an active prompt. The tokens available are:\n"
            "$V shows invis status\n"
            "$D shows the date (for the server, not the game)\n"
            "$T shows the time (for the server, not the game)\n"
            "$C shows the command number\n"
            "$H shows maximum health points\n"
            "$h shows current health points\n"
            "$G shows maximum magic points\n"
            "$g shows current magic points\n"
            "$I shows maximum stamina points\n"
            "$i shows current stamina points\n"
            "$M shows the mud name\n"
            "$N shows the player's name\n"
            +cre+
            "\nFor tactical info as a player:\n" 
            "prompt hp:$h/$H mp:$g/$G sp:$i/$I > \n"
            "or: prompt status\n\n"
            "You can colorize elements of the prompt with the "
            "appropriate color tags, for example:\n"
            "prompt %%^^RED%%^^$M %%^^RESET%%^^ >\n"
            "\nSee also: reprompt, env, colors");
}
