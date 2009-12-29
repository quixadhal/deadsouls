#include <daemons.h>
#include <lib.h>

inherit LIB_COMMAND;

int cmd(string str){
    write("You snap your fingers, and permit hostilities to resume.");
    say(this_player()->GetCapName()+" snaps "+possessive()+" fingers "+
            "and permits hostilities to resume.");
    foreach(object combatant in get_livings(environment(this_player()))){
        combatant->eventUnQuell();
        if(combatant->GetInCombat()) tell_object(combatant,"You resume fighting.");
    }

    return 1;
}

string GetHelp(){
    return "Syntax: unquell\n\n"
        "Allows quelled combat to resume.\n"
        "See also: quell";
}
