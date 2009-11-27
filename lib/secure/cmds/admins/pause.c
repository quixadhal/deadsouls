#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object ob;
    args = lower_case(args);
    if(!user_exists(args)) return capitalize(args) + " is not a member of " +
        possessive_noun(mud_name()) + " reality.";
    WEB_SESSIONS_D->EndSession(args);
    if(!ob = find_player(args)){
        if(member_array(args, PLAYERS_D->GetPendingUnpauses()) != -1){
            PLAYERS_D->RemovePendingUnpause(args);
            write("Pending unpause for "+capitalize(args)+" removed.");
        }
        if(PLAYERS_D->GetPlayerData(args, "Paused")){
            return capitalize(args) + " is already paused.";
        }
        if(member_array(args, PLAYERS_D->GetPendingPauses()) != -1){
            return capitalize(args) + " is already pending a pause.";
        }
        PLAYERS_D->AddPendingPause(args);
        write(capitalize(args)+" will be paused next time they log in.");
        return 1;
    }
    if(ob == this_player()){
        write("You can't pause yourself.");
        return 1;
    }
    if(ob->GetPlayerPaused()){
        write(capitalize(args) + " is already paused!");
        return 1;
    }
    write("You pause "+capitalize(args)+".");
    ob->SetPlayerPaused(1);
    ob->eventPrint("You have been paused. Commands will not work until you are unpaused.");
    return 1;
}

string GetHelp(){
    return ("Syntax: pause <PLAYER>\n\n"
            "Causes the player to be unable to run any commands. Useful for "
            "immediately halting problem behavior.");
}
