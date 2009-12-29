/*    /cmds/creator/trans.c
 *    from the Foundation II LPC Library
 *    moves a player to your location
 *    created by Descartes of Borg 950425
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object ob;

    if( !args || args == "" ) return "Trans whom?";
    if( !(ob = find_player(convert_name(args))) && !(ob = find_living(args)) )
        return "No such being exists anywhere presently.";
    if( environment(ob) == environment(this_player()) ) 
        return ob->GetCapName() + " is right here.";
    if(archp(ob) && !archp(this_player())){
        write("You can't trans an admin.");
        tell_player(ob, this_player()->GetName()+" just tried to trans you.");
        return 1;
    }
    ob->SetProperty("ReturnSite",base_name(environment(ob)));
    message("system", "You have been summoned by " + 
            this_player()->GetName() + ".", ob);
    if( !(ob->eventMoveLiving(environment(this_player()))) )
        return "Failed to move " + ob->GetCapName() + ".";
    else message("system", "You trans " + ob->GetCapName() + 
            " to you.", this_player());
    return 1;
}

string GetHelp() {
    return ("Syntax: trans <living>\n\n"
            "Brings a living thing to your location.\n"
            "See also: return, goto, move, expel");
}
