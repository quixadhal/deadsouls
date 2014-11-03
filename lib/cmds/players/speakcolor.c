#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    if( !sizeof(str) )
        return "Syntax: speakcolor <color>";
    this_player()->SetSpeakColor(str);
    write("%^BOLD%^"+this_player()->GetSpeakColor()+"This is your current color for "+
            "receiving spoken messages.");
    return 1;
}

string GetHelp() {
    return ("Syntax: speakcolor [ cyan | red | green | orange | yellow | blue | magenta | black | white ]\n"
            "        speakcolor [ b_cyan | b_red | b_green | b_orange | b_yellow | b_blue | b_magenta | b_black | b_white ]\n\n"
            "This command allows you to set the color in which you receive spoken messages.");
}
