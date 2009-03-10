/*    /verbs/items/buy.c
 *    From the Dead Souls Mud Library
 *    Allows players to buy things from living vendors
 *    Created by Descartes of Borg sometime in 1996
 *    Version: @(#) buy.c 1.2@(#)
 *    Last modified: 97/01/03
 */

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("buy");
    SetSynonyms("purchase");
    SetRules("STR from LIV");
    SetErrorMessage("Buy what from whom?");
    SetHelp("Syntax: <buy ITEM from VENDOR>\n\n"
            "When in the presence of vendors, you may buy and sell goods "
            "which match the type of goods the vendor in question trades in.  "
            "The \"buy\" command naturally allows you to buy the items "
            "the vendor has for sale.\n\n"
            "Synonyms: purchase\n\n"
            "See also: ask, sell, vendors");
}

mixed can_buy_str_from_liv(string str) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

mixed do_buy_str_from_liv(string str, object vendor) {
    return vendor->eventSell(this_player(), remove_article(lower_case(str)));
}
