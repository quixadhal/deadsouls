#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;


int cmd(string str){
    write("You casually wave your hand, and bring about peace.");
    say(this_player()->GetCapName()+" waves "+possessive()+" hand "+
            "and brings peace to the area.");
    foreach(object combatant in get_livings(environment(this_player()))){
        if(combatant->GetInCombat()) tell_object(combatant,"You stop fighting.");
        combatant->eventQuell();
    }

    return 1;
}

string GetHelp(string str) {
    return "Syntax: <quell>\n"
        "This command brings peace between combatants in your environment.\n"
        "See also: unquell\n";
}
