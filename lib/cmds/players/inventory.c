/*    /lib/inventory.c
 *    from the Dead Souls LPC Library
 *    gives a person information about their inventory
 *    created by Descartes of Borg 950412
 */

#include <lib.h>
#include <rounds.h>
#include <message_class.h>

inherit LIB_DAEMON;

void eventInventory();

    mixed cmd(string args) {
        if( this_player()->GetInCombat() )
            this_player()->SetAttack(0, (: eventInventory :), ROUND_OTHER);
        else eventInventory();
        return 1;
    }

void eventInventory() {
    mapping borg;
    string *items, *wieldeds, *worns, *shorts;
    string ret;
    int i;

    items = map(filter(all_inventory(this_player()), 
                (: !($1->GetInvis(this_player())) &&
                 !($1->GetWorn()) :)),
            (: $1->GetEquippedShort() :));
    wieldeds = map(filter(all_inventory(this_player()),
                (: !($1->GetInvis(this_player())) &&
                 ($1->GetWielded()) :)),
            (: $1->GetEquippedShort() :));
    worns = map(filter(all_inventory(this_player()),
                (: !($1->GetInvis(this_player())) &&
                 !($1->GetWielded()) && $1->GetWorn() :)),
            (: $1->GetEquippedShort() :));
    shorts = items + wieldeds + worns;
    if( !(i = sizeof(shorts)) ) {
        message("system", "You are carrying nothing.", this_player());
        return;
    }
    if( i == 1 ) ret = "You are carrying just this one item:\n";
    else ret = "You are carrying the following items:\n";
    foreach(shorts in ({ items, wieldeds, worns })){
        i = sizeof(shorts);
        if(!i) continue;
        borg = ([]);
        while(i--) if( shorts[i] ) borg[shorts[i]]++;
        i = sizeof(shorts = keys(borg));
        while(i--) ret += capitalize(consolidate(borg[shorts[i]], 
                    shorts[i]))+"\n";
    }
    message("look", ret, this_player());
    if(!this_player()->GetInvis() && 
            !environment(this_player())->GetProperty("meeting room"))
        message(MSG_ANNOYING, this_player()->GetName() + " checks " +
                possessive(this_player()) + " possessions.", 
                environment(this_player()), ({ this_player() }));
}

string GetHelp() {
    return ("Syntax: inventory\n\n"
            "Lists all items you are carrying currently. This command "
            "will take up one round of combat if you happen to be in "
            "combat.");
}
