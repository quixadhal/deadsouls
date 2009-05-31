/*    /spells/heal.c
 *    From Dead Souls LPMud
 *    Created by Faldir 961101
 *    Version: @(#) heal.c 1.6@(#)
 *    Last modified: %D%
 */

#include <lib.h>
#include <daemons.h>
#include <magic.h>
#include <damage_types.h>
#include <magic_protection.h>

inherit LIB_SPELL;

static void create() {
    spell::create();
    SetSpell("healing");
    SetRules("for LIV");
    SetSpellType(SPELL_HEALING);
    SetRequiredMagic(300);
    SetSkills(([ "healing" : 1 ]));
    SetMagicCost( 20, 10);
    SetDifficulty(1);
    SetVerb("pray");
    SetHelp("Syntax: <pray for healing for LIV>\n\n"
            "This minor healing spell provides some minimal "
            "aid to the wounded.");
}

int eventCast(object who, int level, mixed limbs, object array targets) {
    object target = targets[0];
    int feedback,health,magic,stamina,tot;
    if(!spell::eventCast(who,level,targets)) return 0;
    if( target == who ) {
        //who->eventPrint("You can't cast heal on yourself!");
        //return 0;
    }
    else {
        int hp = who->GetSkillLevel("healing");
        int stam = who->GetSkillLevel("faith");
        send_messages("", "$agent_possessive_noun spell partially heals "
                "$target_name.",who,
                target, environment(target));
        health = target->GetMaxHealthPoints()-target->GetHealthPoints();
        stamina = target->GetMaxStaminaPoints()-target->GetStaminaPoints();
        if(health) target->AddHP(((hp > health) ? health : hp));
        if(stamina) 
            target->AddStaminaPoints(((stam > stamina) ? stamina : stam));
        tot = hp+stam;
        if(tot) 
            who->eventTrainSkill("healing",0,0,1,tot);
        magic = who->GetMagicPoints();
        magic = magic - tot;
        if(magic < 0) feedback = tot - magic;
        who->AddMagicPoints(-tot);
        if(feedback){
            write("Your spell overreached your mana reserves! You suffer "+
                    "from a feedback overload!");
            who->eventReceiveDamage(who, KARMA, tot, 1);
        }
    }
    return 1;
} 

varargs int CanCast(object who, int level, string limb, object array targets){
    foreach(object targ in targets){
        string race = targ->GetRace();
        if(race && RACES_D->GetNonMeatRace(race)){
            write("This prayer only works on flesh-and-blood creatures.");
            return 0;
        }
    }
    return ::CanCast(who, level, limb, targets);
}
