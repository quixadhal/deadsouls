/*    /spells/fireball.c
 *    From Dead Souls LPMud
 *    Created by Descartes of Borg 961027
 *    Version: @(#) fireball.c 1.7@(#)
 *    Last modified: 96/11/02
 */

#include <lib.h>
#include <magic.h>
#include <damage_types.h>

inherit LIB_SPELL;

static void create() {
    spell::create();
    SetSpell("fireball");
    SetRules("", "LIV");
    SetSpellType(SPELL_COMBAT);
    SetRequiredMagic(50);
    SetSkills(([ "conjuring" : 8, "magic attack" : 0 ]));
    SetMagicCost(10, 8);
    SetDifficulty(10);
    SetMorality(-20);
    SetAutoDamage(0);
    SetDamage(MAGIC|HEAT, 20, 15);
    SetMessages(({ ({ "accept", "$target_name $target_verb "
				"$agent_possessive_noun fireball with a "
				"smile." }),
			 ({ "are", "$target_name $target_verb slightly "
				"charred by $agent_possessive_noun "
				"fireball." }),
			 ({ "", "$agent_possessive_noun "
				"%^RED%^fireball%^RESET%^ "
				"lights $target_name on fire!" }),
			 ({ "are", "$target_name $target_verb burned to "
				"a crisp by $agent_possessive_noun "
				"%^BOLD%^RED%^fireball%^RESET%^." }) }));
    SetHelp("Syntax: <cast fireball>\n"
	    "        <cast fireball on LIVING>\n\n"
	    "This spell creates a conjures a tremendous fireball out of "
	    "thin air and sends it hurling towards an unsuspecting enemy.  "
	    "If cast without specifying a target while in combat, the "
	    "fireball is targetted at your current enemy.");
}
