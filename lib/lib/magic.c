/*    /lib/magic.c
 *    From Dead Souls LPMud
 *    A module for allowing living beings to cast magic spells.
 *    Created by Descartes of Borg 961027
 *    Version: @(#) magic.c 1.13@(#)
 *    Last modified: 96/11/03
 */

#include <daemons.h>
#include <rounds.h>
#include <magic.h>

private mapping SpellBook = ([]);

// abstract methods
int GetInCombat();
int GetMagicPoints();
string GetName();
int GetParalyzed();
varargs mixed eventPrint(string msg, mixed args...);
int GetSkillLevel(string skill);
int GetStaminaPoints();
void SetAttack(object array e, function f, int type);
// end abstract methods

static varargs void eventCast(object spell, string limb, object array targs);
static void eventTrainSpell(object spell);

mapping GetSpellBook(){
    return copy(SpellBook);
}

void SetSpellBook(mapping book){
    SpellBook = book;
}

int GetSpellLevel(string spell){
    return SpellBook[spell];
}

varargs mixed CanCast(object spell){
    string tmp;
    if( GetParalyzed() ){
        return "You cannot do anything.";
    }
    if( environment()->GetProperty("no magic") ){
        return "Supernatural forces prevent your magic.";
    }
    if( !spell ){
        return "No such spell exists in this reality.";
    }
    if( spell->GetVerb() == "pray" ){
        tmp = "prayer";
    }
    else if( spell->GetVerb() == "perform" ){
        tmp = "feat";
    }
    else {
        tmp = "spell";
    }
    if( !this_object()->GetSpellBook()[spell->GetSpell()] ){
        return "You have never heard of that " + tmp + " before.";
    }
    if( GetMagicPoints() < spell->GetRequiredMagic() ){
        return "You do not have the power required.";
    }
    if( GetStaminaPoints() < spell->GetRequiredStamina() ){
        return "You are too tired.";
    }
    foreach(string skill in spell->GetSkills()){
        if( GetSkillLevel(skill) < spell->GetSkillRequired(skill) ){
            return "That " + tmp + " is beyond your comprehension.";
        }
    }
    return 1;
}

varargs mixed eventPrepareCast(string verb, mixed array args...){
    object spell = SPELLS_D->GetSpell(verb = lower_case(verb));
    object array targets, send_to;
    string special, arg;
    mixed tmp;
    int type;
    if(!spell) spell = SPELLS_D->GetPrayer(verb = lower_case(verb));
    if(spell) tmp = spell->eventParse(this_object(), args...);
    if( !arrayp(tmp) ){
        if( stringp(tmp) ){
            eventPrint(tmp);
        }
        else {
            eventPrint("You are confused.");
        }
        return 1;
    }
    args = tmp;
    targets = spell->GetTargets(this_object(), args...);
    args = filter(args, (: stringp :));
    if( spell->GetAutoHeal() == 0 ){
        if( !sizeof(args) ){
            object array existing = filter(targets, (: $1 :));

            if( sizeof(existing) != 1 ){
                error("This spell was poorly constructed.");
            }
            arg = existing[0]->GetRandomLimb("torso");
        }
        else {
            arg = args[0];
        }
    }
    else {
        if( !sizeof(args) ){
            arg = 0;
        }
        else {
            arg = args[0];
        }
    }
    if( spell->GetVerb() == "pray" ){
        special = "a prayer";
    }
    else if( spell->GetVerb() == "perform" ){
        special = "a mantra";
    }
    else {
        special = "an incantation";
    }
    if( targets ){
        send_to = filter(targets, (: environment($1) == environment() :));
    }
    else {
        send_to = 0;
    }
    send_messages(({ "close", "begin" }),
            "$agent_name $agent_verb $agent_possessive eyes and "
            "$agent_verb uttering " + special + ".", this_object(),
            send_to, environment());
    type = spell->GetSpellType();
    if( GetInCombat() || (type == SPELL_COMBAT) ){
        if( type == SPELL_COMBAT ){
            SetAttack(targets, (: eventCast($(spell), $(arg), $(targets)) :),
                    ROUND_MAGIC);
        }
        else {
            SetAttack(0, (: eventCast($(spell), $(arg), $(targets)) :),
                    ROUND_MAGIC);
        }
    }
    else {
        eventCast(spell, arg, targets);
    }
    return 1;
}

static varargs void eventCast(object spell, string limb, object array targs){
    string name = spell->GetSpell();

    if( this_object()->GetSpellBook()[name] < 100 ){
        eventTrainSpell(spell);
    }
    if( spell->CanCast(this_object(), this_object()->GetSpellBook()[name], limb, targs) ){
        spell->eventCast(this_object(), this_object()->GetSpellBook()[name], limb, targs);
    }
}

mixed eventLearnSpell(string spell){
    object magic = SPELLS_D->GetSpell(spell = lower_case(spell));
    if(!magic){
        write("That's not a spell being offered to you for learning.");
        return 0;
    }

    foreach(string skill in magic->GetSkills()){
        if( magic->GetRequiredSkill(skill) > GetSkillLevel(skill) ){
            return 0;
        }
    }
    if( !this_object()->GetSpellBook()[spell] ){
        SpellBook[spell] = 1;
    }
    return 1;
}

static void eventTrainSpell(object spell){
    string name = spell->GetSpell();
    int x = SpellBook[name] + 1;

    foreach(string skill in spell->GetSkills() ){
        if( (5 * GetSkillLevel(skill)) < x ){
            return;
        }
    }
    SpellBook[name] = x;
}
