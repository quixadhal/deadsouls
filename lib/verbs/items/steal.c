/*    /verbs/items/steal.c
 *    from the Dead Souls V Object Library
 *    steal WRD from LIV
 *    steal OBJ from LIV
 *    created by Descartes of Borg 951218
 */

#pragma save_binary

#include <lib.h>
#include <rounds.h>
#include "include/steal.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("steal");
    SetRules("OBJ from LIV", "WRD from LIV", "OBS from LIV");
    SetErrorMessage("Steal what from whom?");
    SetHelp("Syntax: <steal money from LIVING>\n"
	    "        <steal ITEM from LIVING>\n"
	    "        <steal all [of ITEMS] from LIVING>\n\n"
	    "Uses your stealing abilities to rob another of items or money.  "
	    "Items are much harder to steal than money, and trying to steal "
	    "multiple items in one shot is dangerous.");
}

mixed can_steal_wrd_from_liv(string wrd) {
    if( wrd != "money" ) return 0;
    if( (int)this_player()->GetSkillLevel("stealing") < 1 )
        return "You are not skillful enough at stealing.";
    if( (int)environment(this_player())->GetProperty("no steal") )
      return "Mystical forces prevent your thievery.";
    if( (int)this_player()->GetStaminaPoints() < 10 )
      return "You are too tired for such skullduggery.";
    return 1;
}

mixed can_steal_obj_from_liv() {
    if( (int)this_player()->GetSkillLevel("stealing") < 1 )
        return "You are not skillful enough at stealing.";
    if( (int)environment(this_player())->GetProperty("no steal") )
      return "Mystical forces prevent your thievery.";
    if( (int)this_player()->GetStaminaPoints() < 20 )
      return "You are too tired for such skullduggery.";
    return 1;
}

mixed do_steal_wrd_from_liv(string wrd, object liv) {
    this_player()->eventPrint("You eye " + (string)liv->GetName() +
			      " with thoughts on " + possessive(liv) +
			      " pockets.");
    if( (int)this_player()->GetInCombat() )
      this_player()->SetAttack(0, (: eventSteal,this_player(), "money", liv :),
			       ROUND_OTHER);
    else eventSteal(this_player(), "money", liv);
    return 1;
}

mixed do_steal_obj_from_liv(object item, object liv) {
    if( environment(item) != liv ) {
        this_player()->eventPrint((string)liv->GetName() + " does not have that.");
        return 1;
    }
    this_player()->eventPrint("You eye " + (string)liv->GetName() +
			      " with thoughts on " + possessive(liv) + " " +
			      remove_article((string)item->GetShort()) + ".");
    if( (int)this_player()->GetInCombat() )
      this_player()->SetAttack(0, (: eventSteal, this_player(), ({ item }),
				   liv :), ROUND_OTHER);
    else eventSteal(this_player(), ({ item }), liv);
    return 1;
}

mixed do_steal_obs_from_liv(mixed *res, object liv) {
    object *obs;

    obs = filter(res, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

	ua = unique_array(res, (: $1 :));
	foreach(mixed *lines in ua) this_player()->eventPrint(lines[0]);
	return 1;
    }
    this_player()->eventPrint("You eye " + (string)liv->GetName() +
			      " with thoughts on " + possessive(liv) +
			      " possessions.");
    if( (int)this_player()->GetInCombat() )
      this_player()->SetAttack(0, (: eventSteal, this_player(), obs, liv :),
			       ROUND_OTHER);
    else eventSteal(this_player(), obs, liv);
    return 1;
}

static void eventSteal(object who, mixed what, object target) {
    who->eventSteal(who, what, target);
}
