/*    /spells/buffer.c
 *    From Dead Souls LPMud
 *    Created by Blitz 
 *    Converted to new spell system by BillGates 961119
 *    Version: @(#) buffer.c 1.6@(#)
 *    Last modified: 96/11/10
 */

#include <lib.h>
#include <magic.h>
#include <damage_types.h>
#include <magic_protection.h>

inherit LIB_SPELL;

int hitCallback(object who, object agent, int x, class MagicProtection cl);
void endCallback(object who);

static void create() {
    spell::create();
    SetSpell("buffer");
    SetRules("", "LIV");
    SetSpellType(SPELL_DEFENSE);
    SetRequiredMagic(50);
    SetSkills(([ "magic defense" : 1 ]));
    SetMagicCost(40, 40);
    SetDifficulty(50);
    SetMorality(10);
    SetHelp("Syntax: <cast buffer>\n"
	    "	     <cast buffer on LIV>\n\n"
	    "A magical defense spell that allows the caster to create "
          "a glowing shield around the LIVing object named.\n\n");
}

int eventCast(object who, int level, string race, object array targets) {
    class MagicProtection protection;
    object target = targets[0];
    int prot_level, skill, wis;

    wis = who->GetStatLevel("wisdom");
    skill = who->GetSkillLevel("magic defense");
    prot_level = level;
    prot_level *= (random(skill/10) + 1);
    prot_level += random(wis/2);

    protection = new(class MagicProtection);
    protection->bits = BLUNT | BLADE | KNIFE | MAGIC;
    protection->caster = who;
    protection->absorb = 2*prot_level;
    protection->args = level;
    protection->hit = (: hitCallback :);
    protection->end = (: endCallback :);
    target->AddMagicProtection(protection);
    if( target == who ) {
	send_messages("", "A %^BOLD%^CYAN%^translucent magical shield%^RESET%^ "
	   "suddenly appears around $agent_possessive_noun body.", who, 0,
          environment(target));
    }
    else {
	send_messages("", "A %^BOLD%^CYAN%^translucent magical shield%^RESET%^ "
		 "suddenly appears around $target_possessive_noun body.", who,
		  target, environment(target));
    }
    return 1;
}

int hitCallback(object who, object agent, int x, class MagicProtection cl) {
   string str;
   object Caster = cl->caster;
 
   if( (int)agent->GetUndead() ) return 0;
   str = (string)agent->GetName() || (string)agent->GetShort();
   if( who == Caster ) {
      who->eventTrainSkill("magic defense",cl->args,x,1);
    }
   else if( Caster && playerp(who) && Caster != who ) {
      Caster->eventTrainSkill("magic defense",cl->args,0,1);
    } 

  send_messages("", "The %^YELLOW%^magical shield%^RESET%^ around "
		  "$agent_name wavers as $target_name strikes it.",
		  who, agent, environment(who));
  return x;
}
 
void endCallback(object who) {
    if( who ) {
	send_messages("", "The %^BOLD%^CYAN%^magical shield%^RESET%^ around "
       "$agent_name shatters into nothingness.", who, 0, environment(who));
    }
}
