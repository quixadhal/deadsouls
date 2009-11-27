#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    object who = this_player();
    object env = environment(who);
    if(!env){
        write("No environment.");
        return 1;
    }
    write("You casually wave your hand, and bring about peace.");
    say(who->GetCapName()+" waves "+possessive(who)+" hand "+
            "and brings peace to the area.");
    foreach(object combatant in get_livings(env)){
        if(combatant->GetInCombat()){
            tell_object(combatant,"You stop fighting.");
        }
        combatant->eventQuell();
    }
    return 1;
}

string GetHelp(string str) {
    return "Syntax: quell\n\n"
        "This command brings peace between combatants in your environment.\n"
        "See also: unquell";
}
