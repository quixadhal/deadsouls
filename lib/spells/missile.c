/*    /spells/missile.c
 *    From Dead Souls LPMud
 *    Created by Descartes of Borg 961028
 *    Version: %A$
 *    Last modified: 96/11/02
 */

#include <lib.h>
#include <magic.h>
#include <damage_types.h>

inherit LIB_SPELL;

static void create() {
    spell::create();
    SetSpell("missile");
    SetRules("", "LIV");
    SetSpellType(SPELL_COMBAT);
    SetRequiredMagic(5);
    SetSkills(([ "conjuring" : 0, "magic attack" : 0 ]));
    SetMagicCost(5, 3);
    SetDifficulty(0);
    SetMorality(-1);
    SetAutoDamage(0);
    SetDamage(MAGIC|BLUNT, 10, 5);
    SetMessages(({ ({ "are", "$target_name $target_verb "
				   "unharmed by $agent_possessive "
				   "missile." }),
			 ({ "sting", "$agent_possessive_noun missile "
				     "$agent_verb $target_name." }),
			 ({ "slam", "$agent_possessive_noun missile "
				    "$agent_verb $target_name." }) }));
    SetHelp("Syntax: <cast missile>\n"
	    "        <cast missile on LIVING>\n\n"
	    "This spell creates a conjures a magic missile out of "
	    "thin air and sends it hurling towards an unsuspecting enemy.  "
	    "If cast without specifying a target while in combat, the "
	    "missile is targetted at your current enemy.");
}
