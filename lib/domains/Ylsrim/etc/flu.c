/*    /domains/Ylsrim/etc/flu.c
 *    From the Dead Souls V Object Library
 *    An example of how to build a germ
 *    created by Descartes of Borg 961216
 */

#include <lib.h>
#include <damage_types.h>

inherit LIB_GERM;

int cureMe(object who, int amount, string type);
int infectMe(object who);

static void create() {
    germ::create();
    SetId("flu", "influenza");
    SetType("flu");
    SetCommunicable(20);
    SetCure((: cureMe :));
    SetLifeSpan(60);
    SetInfect((: infectMe :));
}

mixed cureMe(object who, int amount, string type) {
    if( amount < 40 ) {
	return 0;
    }
    return 1;
}

int infectMe(object who) {
    if( who->GetRace() == "klingon" ) {
	return 0;
    }
    who->eventPrint("You start feeling a little achy.");
    return 1;
}

int eventSuffer(object who) {
    if( !random(100) ) {
	Destruct();
	return 1;
    }
    switch(random(3)) {
    case 0:
	send_messages("cough", "$agent_name $agent_verb miserably.",
		      who, 0, environment(who));
	break;

    case 1:
	send_messages("", "$agent_possessive_noun red nose runs all over "
		      "$agent_possessive face.", who, 0, environment(who));
	break;

    case 2:
	send_messages("look", "$agent_name $agent_verb miserable.",
		      who, 0, environment(who));
	break;
    }
    who->eventReceiveDamage(0, COLD, random(3) + 1, 1);
    return 1;
}
	
