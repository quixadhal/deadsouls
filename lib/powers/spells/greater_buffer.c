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
#include <function.h>

inherit LIB_SPELL;

int hitCallback(object who, object agent, int x, class MagicProtection cl);
void endCallback(object who);

static void create() {
    spell::create();
    SetSpell("greater buffer");
    SetRules("", "LIV");
    SetSpellType(SPELL_DEFENSE);
    SetRequiredMagic(50);
    SetSkills(([ "magic defense" : 24, "conjuring" : 24 ]));
    SetMagicCost(80, 80);
    SetDifficulty(2);
    SetMorality(10);
    SetHelp("Syntax: <cast greater buffer>\n"
            "	     <cast greater buffer on LIV>\n\n"
            "A magical defense spell that allows the caster to create "
            "a glowing shield around the LIVing object named.\n\n");
}

int eventCast(object who, int level, string race, object array targets) {
    class MagicProtection protection;
    class MagicProtection *Protections;
    object target = targets[0];
    int prot_level, skill, wis, maxprot;

    maxprot = (who->GetMaxHealthPoints()) * 2;

    Protections = target->GetMagicProtection();
    foreach(class MagicProtection tmp in Protections){
        if(!tmp->obname) continue;
        if(tmp->obname == file_name(this_object())){
            if(!(functionp(tmp->hit) & FP_OWNER_DESTED)){ 
                if(target == this_player()){
                    write("You are already protected by a greater buffer!");
                }
                else {
                    write("They are already protected by a greater buffer!");
                }
                return 1;
            }
        }
    }

    wis = who->GetStatLevel("wisdom");
    skill = who->GetSkillLevel("magic defense");
    skill += who->GetSpellLevel("greater buffer");
    prot_level = level * 3;
    prot_level += (skill);
    prot_level += (wis);

    if(prot_level > maxprot) prot_level = maxprot;

    protection = new(class MagicProtection);
    protection->bits = ALL_EXTERNAL_DAMAGE;
    protection->caster = who;
    protection->absorb = prot_level;
    protection->args = level;
    protection->hit = (: hitCallback :);
    protection->end = (: endCallback :);
    protection->obname = file_name(this_object());
    protection->ob = this_object();
    protection->name = "greater magical buffer";
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

    if( agent->GetUndead() ) return 0;
    str = agent->GetName() || agent->GetShort();
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
