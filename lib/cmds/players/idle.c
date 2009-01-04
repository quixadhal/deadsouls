/*  /cmds/players/idle
 *  Simple command which displays named player's idle time
 *  Recreated by Blitz@Dead Souls 960108
 */

mixed cmd(string args) {
    int x;
    object who;

    if( !sizeof(args) ) return "Syntax: idle <user name>";
    args = convert_name(args);
    who = find_player(args);
    if( !who || who->GetInvis() )
        return "Unable to locate anyone by that name.";
    if( !interactive(who) )
        return (string)who->GetName() + " is link dead.";
    x = query_idle(who);
    if( x > 4 ) {
        this_player()->eventPrint(who->GetName()+" has been idle for "+time_elapsed(x)+".");
    }
    else this_player()->eventPrint((string)who->GetName() + " is "
            "not idle.");
    return 1;
}

string GetHelp(string str) {
    return "Syntax: idle <user name>\n\n"
        "Displays named user's idle time.  A user's \"idle time\" is "
        "the amount of time since the named user last sent input "
        "to the MUD.";
}
