// This command is only useful if you own an intermud channel

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string chan, mud;

    if(!archp(previous_object())) return 0;
    if( !args || args == "" || sscanf(args,"%s %s",chan,mud) != 2) {
        return "Huh?";
    }
    mud = INTERMUD_D->GetMudName(mud);
    if(!mud) return "No such mud.";

    if(member_array(chan,INTERMUD_D->GetChannels()) == -1) 
        return "No such channel.";

    else INTERMUD_D->eventWrite( ({ "channel-admin", 5, mud_name(), 
                this_player()->GetKeyName(), INTERMUD_D->GetNameserver(), 
                0, chan, ({ mud }), ({}) }) );

    write("Ban request for "+mud+" from "+chan+" has been sent.");

    return 1;
}

string GetHelp() {
    return ("Syntax: chanban <channel> <mudname>\n\n"
            "This submits to the intermud router a request to ban the "
            "specified mud from a channel you own. If you do not own "
            "the channel you specify, then the router will not honor "
            "your request. Use the \"real\" channel name, rather than "
            "its local nickname, ie imud_code and not intercre.\n"
            "Example: chanban imud_gossip haven\n"
            "See also: chanunban, chancreate, chanremove");
}
