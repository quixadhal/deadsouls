#include <lib.h>
#include <magic.h>
#include <damage_types.h>
#include <magic_protection.h>

inherit LIB_SPELL;

static void create() {
    spell::create();
    SetSpell("meditate");
    SetRules("", "LIV");
    SetSpellType(SPELL_DEFENSE);
    SetRequiredMagic(50);
    SetSkills(([ "magic defense" : 0 ]));
    SetMagicCost(150, 100);
    SetDifficulty(1);
    SetMorality(10);
    SetHelp("Syntax: <cast meditate>\n"
            "A spell that allows the caster to "
            "restore their stamina more quickly.\n\n");
}

int eventCast(object who, int level) {
    int wis, skill, stamina_level;
    object mojo = new("/secure/obj/meditate_mojo");
    wis = who->GetStatLevel("wisdom");
    skill = who->GetSkillLevel("magic defense");
    stamina_level = level;
    stamina_level *= (random(skill/10) + 1);
    stamina_level += random(wis/2);
    stamina_level += 10;

    who->AddStaminaPoints(stamina_level);
    mojo->AddDuration(who->GetSkillLevel("conjuring") + who->GetSpellLevel("meditate"));
    mojo->eventMove(who);

    send_messages("", "A %^BOLD%^MAGENTA%^dark magical field%^RESET%^ "
            "briefly appears around $agent_possessive_noun body.", who, 0,
            environment(who));
    return 1;
}

