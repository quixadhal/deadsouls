/*    /verbs/items/give.c
 *    from the Dead Souls V Object Library
 *    give LIV OBJ
 *    give OBJ to LIV
 *    give LIV WRD WRD
 *    give WRD WRD to LIV
 *    created by Descartes of Borg 950113
 */

#pragma save_binary

#include <lib.h>
#include "include/give.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("give");
    SetRules("LIV WRD WRD", "WRD WRD to LIV", "LIV OBS", "OBS to LIV");
    SetErrorMessage("Give what to whom?");
    SetHelp("Syntax: <give LIVING ITEM>\n"
	    "        <give LIVING ITEMS>\n"
	    "        <give ITEM to LIVING>\n"
	    "        <give ITEMS to LIVING>\n"
	    "        <give LIVING AMOUNT CURRENCY>\n"
	    "        <give AMOUNT CURRENCY to LIVING>\n\n"
	    "This command allows you to give something you have to "
	    "someone else.\n\n"
	    "See also: drop, get, put");
}

mixed can_give_liv_obj() {
    return can_give_obj_to_liv();
}

mixed can_give_obj_to_liv() { return 1; }

mixed can_give_liv_wrd_wrd(object targ, string num, string curr) {
    return can_give_wrd_wrd_to_liv(num, curr, targ);
}

mixed can_give_wrd_wrd_to_liv(string num, string curr, object targ) {
    int amt;
    
    if( (amt = to_int(num)) < 1 ) return "What sort of amount is that?";
    if( amt > (int)this_player()->GetCurrency(lower_case(curr)) )
      return "You don't have that much " + curr + ".";    return 1;
}

mixed do_give_liv_obj(object target, object what) {
    return do_give_obj_to_liv(what, target);
}

mixed do_give_obj_to_liv(object what, object target) {
    if( !((int)what->eventMove(target)) ) {
	this_player()->eventPrint("It is too heavy to be carried.");
	return 1;
    }
    this_player()->eventPrint("You give " + (string)target->GetName() + " " +
			      (string)what->GetShort() + ".");
    target->eventPrint((string)this_player()->GetName() + " gives you " +
		       (string)what->GetShort() + ".");
    environment(this_player())->eventPrint((string)this_player()->GetName() +
					   " gives " +
					   (string)target->GetName() +
					   " " + (string)what->GetShort() +".",
					   ({ this_player(), target }));
    return 1;
}
    
mixed do_give_liv_wrd_wrd(object target, string num, string curr) {
    return do_give_wrd_wrd_to_liv(num, curr, target);
}

mixed do_give_wrd_wrd_to_liv(string num, string curr, object target) {
    int amt;

    amt = to_int(num);
    if( (int)target->AddCurrency(curr, amt) == -1 ) {
	this_player()->eventPrint("You just can't give that money away.");
	return 1;
    }
    if( (int)this_player()->AddCurrency(curr, -amt) == -1 ) {
	target->AddCurrency(curr, -amt);
	this_player()->eventPrint("The amount of money you have is boggled.");
	return 1;
    }
    this_player()->eventPrint("You give " + (string)target->GetName() + " " +
			      amt + " " + curr + ".");
    target->eventPrint((string)this_player()->GetName() + " gives you " +
		       amt + " " + curr + ".");
    environment(this_player())->eventPrint((string)this_player()->GetName() +
					   " gives " + amt + " " + curr +
					   " to " + (string)target->GetName() +
					   ".", ({ target, this_player() }));
    return 1;
}

mixed do_give_liv_obs(object target, mixed *items) {
    return do_give_obs_to_liv(items, target);
}

mixed do_give_obs_to_liv(mixed *items, object target) {
    object *obs;

    if( sizeof(items) < 1 ) {
        this_player()->eventPrint("You don't have any to give.");
	return 1;
    }
    obs = filter(items, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

	ua = unique_array(items, (: $1 :));
	foreach(string *list in ua) this_player()->eventPrint(list[0]);
	return 1;
    }
    foreach(object ob in obs) do_give_obj_to_liv(ob, target);
    return 1;
}
