/*    /lib/trainer.c
 *    from the Dead Souls  Library
 *    created 960320 by Blitz@Dead Souls
 *    Version: @(#) trainer.c 1.3@(#)
 *    Last modified: 96/10/28
 */
 
#include <lib.h>
#include <daemons.h>
#include "include/trainer.h"
 
inherit LIB_SENTIENT;
 
private string array TrainingSkills;
private mapping Students;
 
/**** driver applies ****/
 
static void create() {
    sentient::create();
    TrainingSkills = ({});   
    Students = ([]);
    SetNoClean(1);
    SetCommandResponses( ([
      ({ "train", "teach" }) : (: eventTrain :),
      "default" : (: eventHelp :),
    ]) );
}
 
static void init() {
    string str;
    ::init();
    if( !living(this_player()) ) return;
    str = (string)this_player()->GetKeyName();
    if( Students[str] ) {
        eventForce("speak You will have to start your "             
                   "studies anew, "+(string)this_player()->GetName());
        map_delete(Students, str);
    }
}
 
/**** data manipulation ****/
 
mixed AddTrainingSkills(string *args...) {
    if( !args || !arrayp(args) ) 
        error("Bad argument 1 to AddTrainingSkills.");
    return (TrainingSkills = distinct_array(TrainingSkills + args));
}
 
mixed RemoveTrainingSkills(string *args...) {
    if( !args || !arrayp(args) ) 
        error("Bad argument 1 to RemoveTrainingSkills.");
    TrainingSkills -= args;
    return TrainingSkills;
}
 
string array GetTrainingSkills() { return copy(TrainingSkills); }
 
mapping GetStudents() { return copy(Students); }
 
/**** high-level events ****/
 
int eventHelp(object who, string unused) {
    eventForce("speak I am not sure of what you are "
               "asking " + (string)who->GetName() + ".");
    eventForce("speak My area of expertise covers " +
               conjunction(TrainingSkills) + ".");
    eventForce("speak You can \"ask "+GetKeyName()+" to teach "
               "<SKILL>\" if you have training points.");
    return 1;
}
 
int eventTrain(object who, string verb, string skill) {
    if( !who || environment(who) != environment() ) {
	return 0;
    }
    if( !sizeof(skill) || !sizeof(verb) ) return eventHelp(who, 0);
    if( verb == "train" ) {
	object ob = SPELLS_D->GetSpell(skill);

	if( ob ) {
	    foreach(string sk in ob->GetSkills()) {
		if( GetSkillLevel(sk) < ob->GetRequiredSkill(sk) ) {
		    eventForce("speak I don't know " + skill + ".");
		    return 0;
		}
	    }
	    if( !who->eventLearnSpell(skill) ) {
		eventForce("speak You are no prepared for that spell!");
		return 0;
	    }
	    who->eventPrint(GetName() + " touches your forehead and gives "
			    "you knowledge of " + skill + ".");
	    environment()->eventPrint(GetName() + " touches " +
				      possessive_noun(who) +
				      " forehead and gives " +
				      objective(who) + " knowledge of " +
				      skill + ".", who);
	    return 1;
	}
    }
    skill = lower_case(skill);
    if( creatorp(who) ) {
        who->eventPrint("Creators do not have skills, get a life.");
        return 1;
    }
    if( Students[ (string)who->GetKeyName() ] ) {
        eventForce("speak I am already training you!");
        return 0;
    }
    if( member_array(skill, TrainingSkills) == -1 ) {
        eventForce("speak I know nothing about the art of " +
                   skill + ".");
        return 0;
    }
    if( member_array(skill, 
           (string *)this_player()->GetSkills() ) == -1 ) {
        eventForce("speak You do not appear to be the type "
                   "who would be skilled in " + skill + "!");
        return 0;
    }
    if( (int)this_player()->GetTrainingPoints() < 1 ) {
        eventForce("speak Fool! You need training points for that!");        
        return 0;
    }
    Students[ (string)who->GetKeyName() ] = skill;
    eventStart(who, skill);
    call_out((: ContinueTraining, who, skill, 0 :), TRAINING_WAIT);
    return 1;
}
 
static int ContinueTraining(object who, string skill, int x) {
    if( !present(who, environment()) ) return 0;
    if( !Students[(string)who->GetKeyName()] ) return 0;
    if( x > 4 ) {
        map_delete(Students, (string)who->GetKeyName());
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
 
int eventStart(object who, string skill) {
    who->eventPrint(GetName() + " begins teaching you "
                   "about the skill of " + skill + ".");
    environment()->eventPrint(GetName() + " begins teaching " +
                             (string)who->GetName() + "...", who);
    return 1;
}
 
int eventContinue(object who, string skill, int x) {
    who->eventPrint("You listen intently as " + GetName()
                    + " continues " + possessive(this_object())
                    + " dissertation on " + skill + ".");
    return 1;
}
 
int eventComplete(object who, string skill) {
    who->eventPrint("You feel more adept with your " + skill + ".");
    eventForce("speak I can teach you no more for now, " +
              (string)who->GetName() + ".");
    return 1;
}
