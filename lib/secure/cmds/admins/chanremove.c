// This command is only useful if you own an intermud channel

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string chan) {

    if(!archp(previous_object())) return 0;
    if( !chan || chan == "") {
        return "Huh?";
    }
    if(member_array(chan,INTERMUD_D->GetChannels()) == -1) 
        return "No such channel.";

    else INTERMUD_D->eventWrite( ({ "channel-remove", 5, mud_name(), 
                this_player()->GetKeyName(), INTERMUD_D->GetNameserver(), 
                0, chan }) );

    load_object("/secure/cmds/creators/update")->cmd("/daemon/intermud");

    write("Channel removal request sent.");

    return 1;
}

string GetHelp(){
    return ("Syntax: chanremove <channel>\n\n"
            "Submits to the intermud router a request to remove the "
            "channel specified. If you do not own the channel, the "
            "request will not be honored.\nSee also: "
            "chanban, chanunban, chancreate");
}
