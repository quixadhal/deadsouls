/*    /lib/trainer.c
 *    from the Dead Souls  Library
 *    created 960320 by Blitz@Dead Souls
 *    Version: @(#) trainer.c 1.3@(#)
 *    Last modified: 96/10/28
 */

#include <lib.h>
#include <daemons.h>
#include "include/trainer.h"

private string array TrainingSkills;
private mapping Students;
int NoSpells = 0;
private static object me;

/**** driver applies ****/

void create(){
    me = this_object();;
    TrainingSkills = ({});   
    Students = ([]);
    me->SetNoClean(1);
    me->SetCommandResponses( ([
                "train" : (: eventTrain :),
                "teach" : (: eventTrain :),
                "default" : (: eventHelp :),
                "help" : (: eventHelp :),
                ]) );
    me->SetRequestResponses( ([
                "help" : (: eventHelp :),
                ]) );
}

static void init(){
    string str;
    if( !living(this_player()) ) return;
    str = this_player()->GetKeyName();
    if( Students[str] ){
        me->eventForce("speak You will have to start your "             
                "studies anew, "+this_player()->GetName());
        map_delete(Students, str);
    }
}

int SetNoSpells(int i){
    if(i) NoSpells = 1;
    else NoSpells = 0;
    return NoSpells;
}

int GetNoSpells(){
    return NoSpells;
}

/**** data manipulation ****/

mixed AddTrainingSkills(string *args){
    if( !args ) 
        error("Bad argument 1 to AddTrainingSkills.");
    return (TrainingSkills = distinct_array(TrainingSkills + args));
}

mixed RemoveTrainingSkills(string *args){
    if( !args || !arrayp(args) ) 
        error("Bad argument 1 to RemoveTrainingSkills.");
    TrainingSkills -= args;
    return TrainingSkills;
}

string array GetTrainingSkills(){ return copy(TrainingSkills); }

string Expertise(){
    string tmp, expertises;
    string expertises2 = "";
    mapping spellbook = me->GetSpellBook();

    if(!sizeof(GetTrainingSkills())) return "none";
    else if(sizeof(GetTrainingSkills()) == 1){
        return GetTrainingSkills()[0];
    }
    expertises = implode(GetTrainingSkills(), ", ");
    if(sizeof(GetTrainingSkills()) == 2){
        expertises = replace_string(expertises,", "," and ");
    }
    else if(sizeof(GetTrainingSkills()) > 2){
        tmp = last_string_element(expertises,",");
        expertises = replace_string(expertises, tmp, " and"+tmp);
    }

    if(sizeof(spellbook) && !GetNoSpells()){
        expertises2 = implode(keys(spellbook), ", ");
        if(sizeof(spellbook) == 1) expertises2 = replace_string(expertises2,", ",".");
        else if(sizeof(spellbook) == 2){
            expertises2 = replace_string(expertises2,", "," and ",1);
            expertises2 = replace_string(expertises2,", ",".");
        }
        else {
            tmp = last_string_element(expertises2,",");
            expertises2 = replace_string(expertises2,tmp," and"+tmp);
        }
        expertises2 = ". In terms of spells, I can teach you "+expertises2;
    }

    return expertises + expertises2;
}

mapping GetStudents(){ return copy(Students); }

/**** high-level events ****/

int eventHelp(object who, string unused){
    if(who) me->eventForce("speak I am not sure of what you are "
            "asking, " + who->GetName() + ".");
    if(sizeof( GetTrainingSkills() )){
        me->eventForce("speak My area of training expertise covers " +
                Expertise() + ".");
        me->eventForce("speak You can \"ask "+me->GetKeyName()+" to train "
                "<SKILL>\" if you have training points.");
        if(sizeof(me->GetSpellBook()) && !GetNoSpells()){
            me->eventForce("speak You can also \"ask "+me->GetKeyName()+
                    " to teach <SPELL>.\"");
        }
    }
    return 1;
}

int eventTrain(object who, string verb, string skill){
    mixed *langs;
    int ok;
    if( !who || environment(who) != environment() ){
        return 0;
    }
    langs = this_object()->GetLanguages();
    foreach(string lang in langs){
        if(who->GetLanguageLevel(lang) >= 95){
            ok = 1;
            me->SetDefaultLanguage(lang);
            break;
        }
    }
    if(!ok){
        write("You must be fluent in one of "+me->GetName()+ 
                " languages in order to understand "+possessive(me)+
                " training.");
        return 1;
    }
    if( !sizeof(skill) || !sizeof(verb) ) return eventHelp(who, 0);
    if(first(skill, 3) == "in ") skill = replace_string(skill,"in ","",1);
    if(first(skill, 6) == "me in ") skill = replace_string(skill,"me in ","",1);

    if( verb == "teach"){
        object ob = SPELLS_D->GetSpell(skill);

        if(!sizeof(me->GetSpellBook()) || me->GetNoSpells()){
            me->eventForce("speak I am not able to teach spells. I only train skills.");
            me->eventHelp();
            return 0;
        }
        if(!ob){
            me->eventForce("speak I've never heard of such a spell.");
            return 0;
        }

        if( !who->eventLearnSpell(skill) ){
            me->eventForce("speak You are not prepared for that spell!");
            return 0;
        }

        who->eventPrint(me->GetName() + " touches your forehead and gives "
                "you knowledge of " + skill + ".");
        environment()->eventPrint(me->GetName() + " touches " +
                possessive_noun(who) +
                " forehead and gives " +
                objective(who) + " knowledge of " +
                skill + ".", who);
        return 1;
    }

    if(skill) skill = lower_case(skill);

    if( Students[ who->GetKeyName() ] ){
        me->eventForce("speak I am already training you!");
        return 0;
    }
    if( member_array(skill, me->GetTrainingSkills()) == -1 ){
        me->eventHelp();
        return 0;
    }
    if( member_array(skill, 
                this_player()->GetSkills() ) == -1 ){
        me->eventForce("speak You do not appear to be the type "
                "who would be skilled in " + skill + "!");
        me->eventForce("speak I cannot train you in a skill you don't know at all. You may need to join a guild or a class that enables you to train in this skill.");
        return 0;
    }
    if( this_player()->GetTrainingPoints() < 1 ){
        me->eventForce("speak You need more training points!");        
        return 0;
    }
    Students[ who->GetKeyName() ] = skill;
    eventStart(who, skill);
    call_out((: ContinueTraining, who, skill, 0 :), TRAINING_WAIT);
    return 1;
}

static int ContinueTraining(object who, string skill, int x){
    if( !who || !Students[who->GetKeyName()] ) return 0;
    if( !present(who, environment()) ||
            member_array(who, this_object()->GetEnemies()) != -1){
        map_delete(Students, who->GetKeyName());
        return 0;
    }
    if( x > 4 ){
        map_delete(Students, who->GetKeyName());
        eventComplete(who, skill);
        who->eventTrain(skill, 1);
        return 1;
    } else {
        eventContinue(who, skill, ++x);
        call_out((: ContinueTraining, who, skill, x :), TRAINING_WAIT);
        return 1;
    }
}

/**** message handling events ****/

/*  The three following events are purely *aesthetic*,
 *  Hopefully prolific coders will override them for  
 *  more interesting training techniques. :) 
 */

int eventStart(object who, string skill){
    who->eventPrint(me->GetName() + " begins teaching you "
            "about the skill of " + skill + ".");
    environment()->eventPrint(me->GetName() + " begins teaching " +
            who->GetName() + "...", who);
    return 1;
}

int eventContinue(object who, string skill, int x){
    who->eventPrint("You listen intently as " + me->GetName()
            + " continues " + possessive(me)
            + " dissertation on " + skill + ".");
    return 1;
}

int eventComplete(object who, string skill){
    who->eventPrint("You feel more adept with your " + skill + ".");
    me->eventForce("speak I can teach you no more for now, " +
            who->GetName() + ".");
    return 1;
}
