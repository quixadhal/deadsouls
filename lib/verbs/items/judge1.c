/*  /verbs/spells/judge.c
 *  a skill for players to judge the relative worth of items
 *  created 961120 by Bill Gates@Dead Souls
 */

#include <lib.h>
#include <vendor_types.h>
#include <rounds.h>
#include "include/judge.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("judge");
    SetSynonyms("equate");
   SetRules("OBJ to OBJ");
    SetErrorMessage("What two things would you like to equate?");
    SetHelp("Syntax: equate OBJ to OBJ\n\n"
        "A bargaining ability which allows people to compare the "
	  "relative worth of two items.  The more experienced you are "
	  "at bartering, the better chance you have of correctly judging "
	  "the more valuable item.\n\n"
        "See help: item commands");
}

mixed can_judge_obj_to_obj() {
  if( this_player()->GetLevel() < 6 ) {
    this_player()->eventPrint("You are not experienced enough to judge "
				      "the value of items accurately.");
    return 0;
    }
  return this_player()->CanCastMagic(1, "judge");
}


mixed do_judge_obj_to_obj(object obj1, object obj2) {
    	int level;
    	string name1, name2;
    	object caster = this_player();
	if( !((int)caster->GetSkillLevel("bargaining")) ) {
        	return "You do not have the skills to judge items."; }
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
    
   level = caster->GetSkillLevel("bargaining");
   caster->eventPrint("You stare intently at "+name1+" and "+name2+".");
   environment(caster)->eventPrint( (string)caster->GetName() +
             " concentrates on " + name1 + " and " + name2 + ".", caster);
   if( (int)this_player()->GetInCombat() )
        this_player()->SetAttack(0,
   	  (: eventJudge, this_player(), obj1, obj2, level :),
                               ROUND_OTHER);
    	else eventJudge(this_player(), obj1, obj2, level);
    	return 1;
}


int eventJudge(object caster, object obj1, object obj2, int level) {

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
        caster->eventPrint("You are too weary to judge right now.");
        environment(caster)->eventPrint(
            (string)caster->GetName() + " looks tired.", caster);
        return 0;

    }
      caster->AddStaminaPoints(-cost);
	obj1lvl = (int)obj1->GetValue();
	obj2lvl = (int)obj2->GetValue();

  if( level < (5 + random(35))) {

    if(random(7)) {
    if(random(5) < 2) {
                better = obj2->GetShort();
                }
                else better = obj1->GetShort();
        caster->eventPrint("%^BOLD%^%^WHITE%^"
          "You determine that " + better + "%^BOLD%^%^WHITE%^"
	" is the more valuable object.%^RESET%^");
        return 0;
        }
	else caster->eventPrint("%^BOLD%^%^WHITE%^"
	  "You determine that these two items are equally valuable."
	  ".%^RESET%^");
    caster->AddSkillPoints("bargaining",random(25));
	return 1;
	}
                                        /* Return the right answer */
      if(obj1lvl == obj2lvl) {
        caster->eventPrint("%^BOLD%^%^WHITE%^"
          "You determine that these two items are equally valuable."
          ".%^RESET%^");
      return 1;
      }
      if(obj1lvl > obj2lvl) {
        better = obj1->GetShort();
        }
      else better = obj2->GetShort();
      caster->eventPrint("%^BOLD%^%^WHITE%^"
        "You determine that " + better + "%^BOLD%^%^WHITE%^"
	" is the more valuable object.%^RESET%^");
caster->AddSkillPoints("bargaining",random(50));
      return 1;

}
