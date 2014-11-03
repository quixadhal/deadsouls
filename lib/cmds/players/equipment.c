#include <lib.h>
#include <rounds.h>

inherit LIB_DAEMON;

void eventInventory();

mixed cmd(string args) {
    if( this_player()->GetInCombat() )
        this_player()->SetAttack(0, (: eventInventory :), ROUND_OTHER);
    else eventInventory();
    return 1;
}

void eventInventory() {
    object *weapons = filter(all_inventory(this_player()), (: $1->GetWielded() :) );
    string *bare_limbs = filter(this_player()->GetLimbs(), (: !sizeof(this_player()->GetWorn($1)) :) );
    object *armors = filter(all_inventory(this_player()), 
            (: $1->GetWorn() && !($1->GetWielded()) :) );
    string ret = "You are using: \n";

    foreach(object weapon in weapons){
        string tmp = capitalize(weapon->GetShort());
        string *limbs = ({});
        foreach(string limb in weapon->GetWorn()){
            limbs += ({ "your "+limb });
        }
        tmp += " wielded in "+item_list(limbs)+".\n";
        ret += tmp+"\n";
    }

    foreach(object armor in armors){
        string tmp = capitalize(armor->GetShort());
        string *limbs = ({});
        foreach(string limb in armor->GetWorn()){
            limbs += ({ "your "+limb });
        }
        tmp += " worn on "+item_list(limbs)+".\n";
        ret += tmp;
    }

    if(sizeof(bare_limbs)){
        ret +="\nYour following body parts are unequipped: \n";
        ret += capitalize(item_list(bare_limbs)+".");
    }

    write(ret);
}

string GetHelp(){
    return ("Syntax: equipment\n\n"
            "Lists all items you are currently wearing or wielding. "
            "This command "
            "will take up one round of combat if you happen to be in "
            "combat.");
}
