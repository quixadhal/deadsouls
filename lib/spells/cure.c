/*    /spells/cure.c
 *    From Dead Souls LPMud
 *    Created by Faldir 961101
 *    Version: @(#) cure.c 1.6@(#)
 *    Last modified: %D%
 */

#include <lib.h>
#include <magic.h>
#include <damage_types.h>
#include <magic_protection.h>

inherit LIB_SPELL;

static void create() {
    spell::create();
    SetSpell("cure");
    SetRules("LIV");
    SetSpellType(SPELL_HEALING);
    SetRequiredMagic(300);
    SetSkills(([ "healing" : 30 ]));
    SetMagicCost( 3, 3);
	SetHealing(3,3);
    SetDifficulty(30);
    SetHelp("Syntax: <cast cure on LIV>\n\n"
	    "This powerful healing spell completely heals the"
     " target at great expense to the caster.");
}

int eventCast(object who, int level, mixed limbs, object array targets) {
    object target = targets[0];
    int health,magic,stamina,tot;
    if(!spell::eventCast(who,level,targets)) return 0;
    if( target == who ) {
	who->eventPrint("You can't cast cure on yourself!");
 return 0;
    }
    else {
	send_messages("", "$agent_possessive_noun spell completely heals "
		  "$target_name!",who,
		      target, environment(target));
        health = target->GetMaxHealthPoints()-target->GetHealthPoints();
        magic = target->GetMaxMagicPoints()-target->GetMagicPoints();
        stamina = target->GetMaxStaminaPoints()-target->GetStaminaPoints();
        if(health) target->eventHealDamage(health);
        if(magic) target->AddMagicPoints(magic);
        if(stamina) target->AddStaminaPoints(stamina);
        tot = health+magic+stamina;
        if(tot) 
          who->eventTrainSkill("healing",0,0,1,tot);
        health -= level * 3;
        stamina -= level * 3;
	send_messages("collapse","%^RED%^$agent_name $agent_verb "
			 "from the effort of casting " 
        "such a spell!%^RESET%^", who,
		      0, environment(target));        
        if(health > who->GetHealthPoints()) 
          who->eventReceiveDamage(who,SHOCK,who->GetHealthPoints()-1);
        else who->eventReceiveDamage(who,SHOCK,health);
        who->AddStaminaPoints(-stamina);
        who->AddMagicPoints(-who->GetMagicPoints());
        who->eventForce("lie down");
        who->SetParalyzed(3+random(3));
    }
    return 1;
} 
