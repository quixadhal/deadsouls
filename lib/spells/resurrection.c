/*    /spells/resurrection.c
 *    From Dead Souls LPMud
 *    Created by BillGates of Money 961204
 *    Version: @(#) resurrection.c 1.2@(#)
 *    Last modified: 96/12/04
 */

#include <lib.h>
#include <magic.h>
#include <damage_types.h>

inherit LIB_SPELL;

static void create() {
    spell::create();
    SetSpell("resurrection");
    SetRules("", "LIV");
    SetSpellType(SPELL_OTHER);
    SetRequiredMagic(75);
    SetRequiredStamina(50);
    SetReligions("Lintokh", "Talima", "Monodeism");
    SetSkills(([ "faith" : 15 ]));
    SetMagicCost(10);    
    SetDifficulty(40);
    SetMorality(5);
    SetHelp("Syntax: <cast refresh>\n"
	    "        <cast refresh on LIVING>\n\n"
	    "This spell allows the caster to \"refresh\" his "
            "or her target's stamina.  The amount of stamina "
            "refreshed depends solely on the magical powers of "
            "the caster.\n\n");
}

varargs int CanCast(object who, int level, string limb, object array targs) {
    object target = targs[0];
    string targ_rel, my_rel;
    
    targ_rel = target->GetReligion(1);
    my_rel = who->GetReligion(1);
    if( !(int)target->GetUndead() ) {
      who->eventPrint((string)target->GetName() + " is not undead!");
      return 0;
    }
    if( (int)who->ClassMember("cleric") ) return 1;
    if( targ_rel != my_rel ) {
	send_messages("are", "$target_name $target_verb not of ",
		        "$agent_possessive faith.",
			  who, target);
	return 0;
    }
    return spell::CanCast(who, level, limb, targs);
}

varargs int eventCast(object who, int level, string limb, object array targs) {
    object target = targs[0];
    int chance = level + random((int)who->GetSkillLevel("healing"));
    if( level < (30 + random(30)) ) {
      target->AddStaminaPoints(-(int)target->GetStaminaPoints());
      who->AddStaminaPoints(-(int)who->GetStaminaPoints());
      who->eventPrint("You feel utterly drained as your "
              "spell of resurrection goes awry.", target);
      target->eventPrint("You feel a draining effect as " +
              possessive_noun(who) + " attempt to resurrect you "
              "goes awry.", who);
      environment(who)->eventPrint( (string)who->GetCapName()+" and "+
              (string)target->GetCapName()+" both double over in pain "
              "as "+possessive_noun(who)+" spell fails.",
              ({ who, target }) );      
      who->AddMagicPoints(-(int)who->GetMagicPoints());
      return 0;
    }

    send_messages(({ "bring" }), "$agent_name $agent_verb "
                                 "$target_name back from the dead.",
                                 who, target, environment(who));
    target->eventRevive();
      who->AddMagicPoints(-(int)who->GetMagicPoints());
    return 1;
}
 
int GetMagicCost() {
  return (int)this_player()->GetMagicPoints();
}
