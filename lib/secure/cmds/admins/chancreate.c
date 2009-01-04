// This command is only useful if you own an intermud channel

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string chan;
    int mode = 0;

    if(!archp(previous_object())) return 0;
    write("This command is for creating Intermud-3 channels. "+
            "You almost certainly do not need to use it. If you are "+
            "100% sure you know what you are doing and in fact do need "+
            "to create an Intermud-3 channel, then please edit this "+
            "command to remove this warning and the return that follows it.");
    return 1;
    if( !args || args == "") return "Huh?";
    if(sscanf(args,"%s %d",chan,mode) != 2) chan = args;
    if(mode > 2 || mode < 0) return "Invalid mode.";

    else INTERMUD_D->eventWrite( ({ "channel-add", 5, mud_name(), 
                this_player()->GetKeyName(), INTERMUD_D->GetNameserver(), 
                0, chan, mode }) );

    load_object("/secure/cmds/creators/update")->cmd("/daemon/intermud");

    write("Channel creation request sent.");

    return 1;
}
