/*    /verbs/items/wear.c
 *    From the Dead Souls Object Library
 *    Allows a player to wear armour
 *    Created by Descartes of Borg 951020
 *    Version: @(#) wear.c 1.2@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>
#include <armour_types.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("wear");
    SetRules("OBS", "OBJ on STR");
    SetErrorMessage("Wear what?  When in doubt, specify a limb to "
		    "wear it on.");
    SetHelp("Syntax: <wear ARMOUR>\n"
	    "        <wear ARMOUR on LIMB>\n\n"
	    "Allows you to take an article of clothing and wear it.  Some "
	    "bits of clothing may be worn in a variety of places, and thus "
	    "require you to specify where it is they should be worn.  For "
	    "example, a shield could be worn with either the right hand or "
	    "left hand.  So you would type, \"wear shield on right hand\".\n\n"
	    "See also: wield");
}

mixed can_wear_obj() {
    if( this_player()->GetParalyzed() ) {
	return "You cannot do anything.";
    }
    return 1;
}

mixed can_wear_obj_on_str(string str) {
    return can_wear_obj();
}

mixed do_wear_obj(object ob) {
    return ob->eventEquip(this_player(), ob->GetRestrictLimbs());
}

mixed do_wear_obj_on_str(object ob, string str) {
    return ob->eventEquip(this_player(),
			  ({ remove_article(lower_case(str)) }));
}

mixed do_wear_obs(object array armours) {
    object array obs;

    if( !sizeof(armours) ) {
        this_player()->eventPrint("There is no such thing to be worn.");
	return 1;
    }
    obs = filter(armours, (: objectp :));
    if( !sizeof(obs) ) {
        mixed array ua;

	ua = unique_array(armours, (: $1 :));
	foreach(string array list in ua) {
	    this_player()->eventPrint(list[0]);
	}
	return 1;
    }
    foreach(object armour in obs) {
	do_wear_obj(armour);
    }
    return 1;
}
