/*  /verbs/players/stealth.c
 *  by BillGates 961109
 *  allows players to move silently from room to room
 */

#include <lib.h>
#include "include/stealth.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("stealth");
    SetRules("","WRD");
    SetHelp("Syntax: stealth [on | off]\n\n"
            "A way to enable or disable sneakiness, if stealth is a "
            "skill available to you.");
}

mixed can_stealth() {
    return this_player()->CanCastMagic(0, "stealth");
}

mixed can_stealth_wrd() {
    return can_stealth();
}

mixed do_stealth() {
    return do_stealth_wrd("");
}

mixed do_stealth_wrd(string args) {
    object caster  = this_player();
    object env = environment(caster);

    int skill = this_agent()->GetSkillLevel("stealth");
    if( skill < 20 ) {
        caster->eventPrint("You are not devious enough.");
        return 1;
    }


    if( args == "on" ) {  
        if( caster->GetStaminaPoints() < 50 ) {
            caster->eventPrint("You are too tired to move silently.",env);
            env->eventPrint(caster->GetName() + " looks tired.",caster);
            return 0;
        }


        if( caster->GetInCombat() ) {
            caster->eventPrint("You are too busy fighting at the moment.",env);
            return 0;
        }
    }	    
    eventStealth(caster, args, skill);
    return 1;
}


int eventStealth(object caster, string args, int skill) {

    if( !(caster) ) return 0;

    if( !environment(caster) ) {
        caster->eventPrint("You are nowhere.");
        return 0;
    }

    if(args == "") {
        if(caster->GetProperty("stealthy")) {
            caster->eventPrint("You are currently moving silently.");
        }
        else caster->eventPrint("You are moving as noisily as ever.");
        return 1;
    }

    if(args == "on") {
        if(caster->GetProperty("stealthy")) {
            caster->eventPrint("You are already moving quietly!");
            return 0;
        }
        caster->SetProperty("stealthy",1);
        caster->eventPrint("%^RED%^You begin to sneak around.");
        caster->AdStaminaPoints(-30 - random(40));
        caster->AddSkillPoints("stealth",skill*skill/8);
        return 1;
    }    

    if(args == "off") {
        if(caster->GetProperty("stealthy")) {
            caster->SetProperty("stealthy",0);
            caster->eventPrint("%^BOLD%^%^CYAN%^You stop your sneaky ways.",
                    environment(caster));
            return 1;
        }
        else caster->eventPrint("You are not currently sneaking around!");
        return 0;
    }  		     

    return 1;
}
