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

    write(mud+" has been banned from "+chan+".");

    return 1;
}
