/*    /domains/Ylsrim/npc/leader.c
 *    from the Dead Soulsr2 Object Library
 *    class leader inheritable
 *    created by Descartes of Borg 951115
 *    Version: @(#) leader.c 1.5@(#)
 *    Last modified: 96/11/24
 */

#include <lib.h>
#include <daemons.h>
#include <message_class.h>
#include "include/leader.h"

inherit LIB_SENTIENT;

int eventTeachPlayer(object who, string spell);

static void create() {
    sentient::create();
}

mixed eventAsk(object who, string str) {
    string cmd, args;
    mixed tmp;
    
    if( (tmp = sentient::eventAsk(who, str)) == 1 ) return 1;
    if( !str || str == "" ) {
	eventForce("speak ask me to what? To describe " + GetClass() + "s?");
	return 1;
    }
    if( sscanf(str, "%s %s", cmd, args) != 2 ) {
	cmd = str;
	args = 0;
    }
    switch(cmd) {
	case "describe": case "preview": case "explain":
	eventPreview(who, args);
	break;

	case "convert":
	eventConvert(who, args);
	break;

	case "join": case "become": case "be":
	eventJoin(who, args);
	break;

        case "teach":
	eventTeachPlayer(who, args);
	break;
	    
	default:
	eventForce("speak I am not sure what you want");
	if( who->GetClass() ) {
	    eventForce("speak do you mean to ask me to teach a spell?");
	}
	else {
	    eventForce("speak do you mean to ask me to describe " +
		       pluralize(GetClass()) + "?");
	}
	break;
    }
    return 1;
}

void eventConvert(object who, string args) {
    if( GetSkillLevel("faith") < 1 ) {
	eventForce("speak I don't do conversions");
	return;
    }
    if( !args || args == "" ) {
	eventForce("speak convert whom?");
	return;
    }
    if( args != "me" ) {
	object ob;

	ob = present(args, environment());
	if( !ob ) {
	    eventForce("speak I don't see any such thing here");
	    return;
	}
	if( !living(ob) ) {
	    eventForce("laugh");
	    eventForce("speak would like to worsip some cheese too?");
	    return;
	}
	if( ob != who ) {
	    eventForce("speak " + (string)ob->GetName() + " must request "
		       "conversion of " + possessive(ob) + " own free will.");
	    return;
	}
    }
    who->SetProperty("converting", GetReligion(1));
    eventForce("speak I will give it a try, I hope my faith servers me");
    call_out((: eventForce("convert " + (string)($(who))->GetKeyName()) :), 1);
}

void eventPreview(object who, string args) {
    if( args ) args = remove_article(lower_case(args));
    if( args && args != "" && args != GetClass() ) {
	if( args[0..<2] != GetClass() ) {
	    eventForce("speak You want me to describe what?");
	    return;
	}
    }
    who->eventPage(DIR_CLASS_HELP "/" + GetClass(), MSG_HELP);
}

void eventJoin(object who, string args) {
    if( !args || args == "" ) {
	eventForce("speak Do you mean you wish to become " +
		   add_article(GetClass()) + "?");
	return;
    }
    args = remove_article(lower_case(args));
    if( args != GetClass() && args[0..<2] != GetClass() && args != pluralize(GetClass()) ) {
	eventForce("speak you want me to make you a what?");
	eventForce("speak people only ask me to join the " +
		   pluralize(GetClass()));
	return;
    }
    if( (int)who->ClassMember(GetClass()) ) {
	eventForce("speak You are already " + add_article(GetClass()));
	return;
    }
    if( (string)who->GetClass() == (string)who->SetClass(GetClass()) ) {
	eventForce("speak You cannot become " + add_article(GetClass()) + "!");
	eventForce("attack " + (string)who->GetKeyName());
	return;
    }
    if( !((string)who->GetReligion()) && GetReligion() )
      who->SetReligion(GetReligion(0), GetReligion(1));
    environment()->eventPrint(GetName() + " makes " + (string)who->GetName() +
			      " " + add_article(GetClass()) + ".",
			      ({ who, this_object() }));
    eventForce("speak welcome new " + GetClass() + "!");
    eventForce("speak Inside this hall, you will find sanctuary.");
    eventForce(GetClass() + " " + (string)who->GetName() +
	       " just joined our ranks!");
    who->SetShort("foo");  /* reset title */
    return;
}

int eventPreAttack(object ob) {
    if( member_array(ob, GetEnemies()) > -1 ) return sentient::eventPreAttack(ob);
    eventForce(GetClass() + " " + pluralize(GetClass()) + "! Our home is " 
	       "being raided by " + (string)ob->GetName() + "!");
    return sentient::eventPreAttack(ob);
}

int eventTeachPlayer(object who, string spell) {
    object ob = SPELLS_D->GetSpell(spell);

    if( ob ) {
	foreach(string skill in ob->GetSkills()) {
	    if( GetSkillLevel(skill) < ob->GetRequiredSkill(skill) ) {
		eventForce("speak I don't know " + spell + ".");
		return 1;
	    }
	}
	if( !who->eventLearnSpell(spell) ) {
	    eventForce("speak You are no prepared for that spell!");
	    return 1;
	}
	who->eventPrint(GetName() + " touches your forehead and gives "
			"you knowledge of " + spell + ".");
	environment()->eventPrint(GetName() + " touches " +
				  possessive_noun(who) +
				  " forehead and gives " +
				  objective(who) + " knowledge of " +
				  spell + ".", who);
	return 1;
    }
    else {
	eventForce("speak I have never heard of such a spell");
	return 1;
    }
}
