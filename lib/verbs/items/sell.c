/*    /verbs/items/sell.c
 *    From the Dead Souls Object Library
 *    Allows a player to sell stuff to a vendor
 *    Created by Descartes of Borg 9602??
 *    Version: @(#) sell.c 1.2@(#)
 *    Last modified: 97/01/03
 */

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("sell");
    SetRules("OBS to LIV", "LIV OBS");
    SetErrorMessage("Sell what to whom?");
    SetHelp("Syntax: <sell ITEM to LIVING>\n\n"
	    "When in the presence of vendors, you may buy and sell goods "
	    "which match the type of goods the vendor in question trades in.  "
	    "The \"sell\" command naturally allows you to sell an item "
	    "to an interested vendor.\n\n"
	    "See also: ask, sell, vendors");
}

mixed can_sell_obj_to_liv() {
    if( this_player()->GetParalyzed() ) {
	return "You cannot do anything.";
    }
    return 1;
}

mixed can_sell_liv_obs() {
    return can_sell_obj_to_liv();
}

mixed do_sell_obj_to_liv(object ob, object vendor) {
    return vendor->eventBuy(this_player(), ({ ob }));
}

mixed do_sell_liv_obj(object vendor, object item) {
    return do_sell_obj_to_liv(item, vendor);
}

mixed do_sell_obs_to_liv(object array items, object vendor) {
    object array obs;

    obs = filter(items, (: objectp :));
    if( !sizeof(obs) ) {
        mixed array ua;

	ua = unique_array(items, (: $1 :));
	foreach(string array list in ua) {
	    this_player()->eventPrint(list[0]);
	}
	return 1;
    }
    return vendor->eventBuy(this_player(), obs);
}

mixed do_sell_liv_obs(object vendor, object array items) {
    return do_sell_obs_to_liv(items, vendor);
}
