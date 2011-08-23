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
    SetSpell("greater fireball");
    SetRules("", "LIV");
    SetSpellType(SPELL_COMBAT);
    SetRequiredMagic(100);
    SetSkills(([ "conjuring" : 24, "magic attack" : 24 ]));
    SetMagicCost(49, 49);
    SetStaminaCost(10, 10);
    SetDifficulty(10);
    SetMorality(-20);
    SetAutoDamage(0);
    SetDamage(MAGIC|HEAT, ({ 60, 45 }) );
    SetMessages(({ 
                ({ "are", "$target_name $target_verb "
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
