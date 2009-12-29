#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    mixed tmp;
    string stat;
    int amt;

    if( !args || args == "" ) {
        amt = this_player()->GetCustomDeviations();
        this_player()->eventPrint("You have " + amt + 
                " deviation points left to use.", MSG_SYSTEM);
        return 1;
    }
    if(args == "enable"){
        int i = this_player()->GetDeviating();
        if(i){
            write("You already have enabled deviation for yourself.");
            return 1;
        }
        i = this_player()->SetDeviating(1);
        if(i) write("Deviation enabled.");
        else write("Deviation not enabled. See an admin for help.");
        return 1;
    }
    if(!this_player()->GetDeviating()){
        write("You have not enabled deviation for yourself.");
        write("Please type: help deviate");
        write("And: help deviation");
        return 1;
    }
    amt = to_int((tmp = explode(args, " "))[<1]);
    if( sizeof(tmp) == 1 ) return "Deviate which stat how much?";
    stat = implode(tmp[0..<2], " ");
    tmp = this_player()->eventDeviateStat(stat, amt);
    if( stringp(tmp) ) return tmp;
    if( !tmp ) return "Failed to deviate.";
    this_player()->eventPrint("Your " + stat + " class is now " + tmp +
            ", and you have " +
            this_player()->GetCustomDeviations() +
            " deviation points left.", MSG_SYSTEM);
    return 1;
}

string GetHelp(){
    return ("Syntax: deviate\n"
            "        deviate enable\n"
            "        deviate STAT AMOUNT\n\n"
            "Changes your \"stat class\" for the specified stat, "
            "meaning that it improves more frequently when your "
            "player level goes up. However it incurs great cost to "
            "you in terms of an XP penalty. You cannot reverse a "
            "deviation nor its penalty. Before proceeding, read the "
            "helpfile for deviation:\nhelp deviation\n\n"
            "If you decide to deviate, first enable it by typing:\n"
            "deviate enable\nSee also: deviation, customize, "
            "stat, stats, score");
}
