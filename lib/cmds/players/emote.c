/*    /cmds/player/emote.c
 *    from the Dead Souls LPC Library
 *    for those times when you are feeling emotional
 *    created by Descartes of Borg 950412
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    if( !creatorp(this_player()) && !avatarp(this_player()) ) {
        if( this_player()->GetStaminaPoints() < 1 )
            return "You are too tired.";
    }
    if( !args || args == "" ) {
        message("my_action", "You are feeling emotional.", this_player());
        message("other_action", this_player()->GetName() +
                " looks emotional.", environment(this_player()),
                ({ this_player() }));
        return 1;
    }
    if( args[0] != '\'' ) args = " " + args;
    message("my_action", "You emote: " + this_player()->GetName() + 
            args, this_player());
    message("other_action", this_player()->GetName() + args,
            environment(this_player()), ({ this_player() }) );
    return 1;
}

string GetHelp() {
    return ("Syntax: emote <message>\n\n"
            "Places any message you specify directly after your name.  For "
            "example, \"emote smiles.\" would have others see "
            "\"Descartes smiles.\".  Non-avatars lose a stamina point for "
            "each emote to discourage abuse.");
}

