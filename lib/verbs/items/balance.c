/*  /verbs/spells/judge.c
 *  a skill for players to judge the relative weight of items
 *  created 96/12/04 by Faldir@Dead Souls
 */

#include <lib.h>
#include <vendor_types.h>
#include <rounds.h>
#include "include/balance.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("balance");
    SetSynonyms("weigh");
   SetRules("OBJ to OBJ");
    SetErrorMessage("What two things would you like to balance?");
    SetHelp("Syntax: balance OBJ to OBJ\n\n"
        "A simple tool for determining which is the heavier of two objects."
        "See help: item commands");
}

mixed can_balance_obj_to_obj() {
  return this_player()->CanCastMagic(1, "balance");
}


mixed do_balance_obj_to_obj(object obj1, object obj2) {
    	string name1, name2;
    	object caster = this_player();
    	if (!obj1 || !obj2) return "You must judge one thing vs another.";
    	if (obj1 == obj2) return "That would do a lot of good!";
/* Check for presence of objects */
    	name1 = obj1->GetShort();
	name2 = obj2->GetShort();
    
    	if( environment(obj1) != caster ) {
    	    caster->eventPrint("You do not have "+name1+".");
    	    return 1;
    	}
    	
    	if( environment(obj2) != caster ) {
    	    caster->eventPrint("You do not have "+name2+".");
    	    return 1;
    	}
    
   caster->eventPrint("You stare intently at "+name1+" and "+name2+".");
   environment(caster)->eventPrint( (string)caster->GetName() +
             " concentrates on " + name1 + " and " + name2 + ".", caster);
   if( (int)this_player()->GetInCombat() )
        this_player()->SetAttack(0,
   	  (: eventBalance, this_player(), obj1, obj2 :),
                               ROUND_OTHER);
    	else eventBalance(this_player(), obj1, obj2);
    	return 1;
}


int eventBalance(object caster, object obj1, object obj2) {

    	int obj1lvl, obj2lvl;
    	string better;

    	int cost = random(50) + 50;

    	if( !(obj1 && obj2) ) return 0;
    	if( (environment(obj1) != caster) || (environment(obj2) != caster) ) {
        caster->eventPrint("You must have both items in your possesion "
        "to compare them.");
        return 0;
    }
    if( cost > (int)caster->GetStaminaPoints() ) {
        caster->eventPrint("You are too weary to balance right now.");
        environment(caster)->eventPrint(
            (string)caster->GetName() + " looks tired.", caster);
        return 0;

    }
      caster->AddStaminaPoints(-cost);
	obj1lvl = (int)obj1->GetMass();
	obj2lvl = (int)obj2->GetMass();

                                        /* Return the right answer */
      if(obj1lvl == obj2lvl) {
        caster->eventPrint("%^BOLD%^%^WHITE%^"
          "You determine that these two items are equally heavy."
          ".%^RESET%^");
      return 1;
      }
      if(obj1lvl > obj2lvl) {
        better = obj1->GetShort();
        }
      else better = obj2->GetShort();
      caster->eventPrint("%^BOLD%^%^WHITE%^"
        "You determine that " + better + "%^BOLD%^%^WHITE%^"
	" is the heavier object.%^RESET%^");
      return 1;

} 
