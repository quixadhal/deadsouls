/*    /verbs/items/wield.c
 *    From the Dead Souls Object Library
 *    The command to allow people to wield weapons
 *    Created by Descartes of Borg 951018
 *    Version: @(#) wield.c 1.2@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>

inherit LIB_VERB;

string array GetFreeLimbs(object who) {
    string array limbs = who->GetWieldingLimbs();
    
    limbs = filter(limbs, (: !$(who)->GetWielded($1) :));
    return limbs;
}
	
static void create() {
    verb::create();
    SetVerb("wield");
    SetRules("OBS", "OBS in STR", "OBS with STR");
    SetErrorMessage("Wield what?  Perhaps you mean to specify in which "
		    "limbs?");
    SetHelp(
        "Syntax: <wield all>\n"
	    "        <wield OBJ>\n"
	    "        <wield OBJ in LIMB>\n"
	    "        <wield OBJ with LIMB>\n"
	    "\n"
	    "This command sets the weapon you name to be wielded in the "
	    "limb you specify.\n\n"
	    "Note that a limb can be a single limb, or a list of limbs "
	    "separated by a comma, or the word \"and\", depending on "
	    "how many limbs are required for wielding the weapon.  For "
	    "example:\n"
	    "\twield the rusty sword with my right hand and left hand\n"
	    "\twield the artrell sword with first hand, second hand, and "
	    "third hand\n"
	    "\n"
	    "See also: wear");
}

mixed can_wield_obj() { 
    if( !sizeof(this_player()->GetWieldingLimbs()) ) {
	return "You have no limbs with which to wield!";
    }
    if( this_player()->GetParalyzed() ) {
	return "You cannot do anything";
    }
    return 1;
}

mixed can_wield_obj_word_str() {
    return can_wield_obj();
}

mixed do_wield_obj(object ob) {
    string array limbs = GetFreeLimbs(this_player());
    int hands = ob->GetHands();
    
    if( hands < sizeof(limbs) ) {
	limbs = limbs[0..(hands-1)];
    }
    return ob->eventEquip(this_player(), limbs);
}

mixed do_wield_obj_word_str(object ob, string word, string str) {
    return ob->eventEquip(this_player(), explode_list(str));
}

mixed do_wield_obs(mixed array targs) {
    object array obs;

    if( !sizeof(targs) ) {
        this_player()->eventPrint("There is no such thing to be wielded.");
	return 1;
    }
    obs = filter(targs, (: objectp :));
    if( !sizeof(obs) ) {
	mapping messages = unique_mapping(targs, (: $1 :));
	
	foreach(string msg in keys(messages)) {
	    this_player()->eventPrint(msg);
	}
	return 1;
    }
    foreach(object item in obs) {
	string array limbs = GetFreeLimbs(this_player());
	int hands = item->GetHands();

	if( sizeof(limbs) < hands ) {
	    this_player()->eventPrint("You don't have anywhere to wield " +
				      item->GetDefiniteShort() + ".");
	}
	else {
	    mixed tmp;
	    
	    if( hands < sizeof(limbs) ) {
		limbs = limbs[0..(hands-1)];
	    }
	    tmp = item->CanEquip(this_player(), limbs);
	    if( tmp != 1 ) {
		if( !tmp ) {
		    this_player()->eventPrint("You cannot wield " +
					      item->GetDefiniteShort() + ".");
		}
		else {
		    this_player()->eventPrint(tmp);
		}
	    }
	    else {
		item->eventEquip(this_player(), limbs);
	    }
	}
    }
    return 1;
}
