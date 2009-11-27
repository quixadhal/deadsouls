#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object ob;
    args = lower_case(args);
    if(!user_exists(args)) return capitalize(args) + " is not a member of " +
        possessive_noun(mud_name()) + " reality.";
    if(!ob = find_player(args)){
        if(member_array(args, PLAYERS_D->GetPendingPauses()) != -1){
            PLAYERS_D->RemovePendingPause(args);
            write("Pending pause for "+capitalize(args)+" removed.");
        }
        if(!(PLAYERS_D->GetPlayerData(args, "Paused"))){
            return capitalize(args) + " is already unpaused.";
        }
        if(member_array(args, PLAYERS_D->GetPendingUnpauses()) != -1){
            return capitalize(args) + " is already pending an unpause.";
        }
        PLAYERS_D->AddPendingUnpause(args);
        write(capitalize(args)+" will be unpaused next time they log in.");
        return 1;
    }
    if(ob == this_player()){
        write("You can't unpause yourself.");
        return 1;
    }
    if(!(ob->GetPlayerPaused())){
        write(capitalize(args) + " is already unpaused!");
        return 1;
    }
    write("You unpause "+capitalize(args)+".");
    ob->SetPlayerPaused(0);
    ob->eventPrint("You have been unpaused. Commands will work now.");
    return 1;
}

string GetHelp(){
    return ("Syntax: unpause <PLAYER>\n\n"
            "Causes the player to be able to run commands "
            "after being paused.");
}
