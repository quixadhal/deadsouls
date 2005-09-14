/*    /domains/Ylsrim/etc/cold.c
 *    From the Dead Souls V Object Library
 *    An example of how to build a germ
 *    created by Descartes of Borg 961216
 */

#include <lib.h>
#include <damage_types.h>

inherit LIB_GERM;

static void create() {
    germ::create();
    SetId("cold");
    SetType("cold");
    SetCommunicable(40);
    SetCure(20);
    SetLifeSpan(60);
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
	
