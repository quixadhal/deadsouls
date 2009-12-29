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

string GetHelp(){
    return ("Syntax: chancreate <name> [0 | 1 | 2]\n\n"
            "This submits to the intermud router a request to create an "
            "intermud channel by the name you specify with the mode you "
            "specify. If an intermud channel by that name already exists, "
            "your request will not be honored. Otherwise after a few minutes "
            "of updating and broadcasting, your new channel should be "
            "available. The mode indicates whether it is an invite-only "
            "channel. For example: \nchancreate macrame 1\n"
            "would create a channel called macrame that no mud can use "
            "until you explicitly allow it. On the other hand,"
            "\nchancreate dramallamas\n would create a channel any mud "
            "can join until you explicitly chanban them. Mode 2, or "
            "\"filtered\" mode, is an advanced topic left as an exercise "
            "to the reader."
            "\nSee also: chanban, chanunban, chanremove, "
            "http://www.intermud.org/specs/i3.html");
}
