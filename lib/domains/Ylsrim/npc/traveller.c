/*    /domains/Ylsrim/npc/traveller.c
 *    From the Dead Souls V Object Library
 *    An example of a wandering sentient who steals
 *    Created by Descartes of Borg 960302
 */

#include <lib.h>

inherit LIB_SENTIENT;

// This steal function will get called randomly as set up in
// the SetAction() function in create().  It checks to see if
// someone is around from whom this guy can steal
static void Steal() {
    object array obs;
    object target;
    string cmd;
    int i;

    if( !environment() ) {
	return;
    }
    // Find all living things that are not me in the same room as me
    obs = filter(all_inventory(environment()),
		 (: living($1) && $1 != this_object() :));
    i = sizeof(obs);
    if( i == 0 ) { // no one to steal from
	return;
    }
    if( i != 1 ) { // many potential targets
	// pick one
	i = random(i);
    }
    else { // only oone target, so pick obs[0]
	i = 0;
    }
    target = obs[i];
    // Find all items on the target that can be stolen
    obs = filter(all_inventory(target), 
		 (: !($1->GetProperty("no steal")) :));
    i = sizeof(obs);
    if( i == 0 ) { // no goods, let's try to get some cash
	cmd = "pick " + target->GetKeyName();
    }
    else { // heh, some goodies
	if( i != 1 ) { // many goodies
	    // pick one
	    i = random(i);
	}
	else {
	    i = 0;
	}
	cmd = "steal " + obs[i]->GetKeyName() + " from " +
	    target->GetKeyName();
    }
    // now do the steal or pick
    eventForce(cmd);
}

static void create() {
    sentient::create();
    SetKeyName("traveller");
    SetId("traveller");
    SetShort("a traveller");
    SetLong("He looks a bit worn down, but also a bit shifty.");
    SetLevel(5);
    SetRace("artrell");
    SetClass("fighter");
    SetGender("male");
    SetMorality(-200);
    AddCurrency("electrum", 1 + random(100));
    SetInventory(([ "/domains/Ylsrim"+ "/armour/artrell_armour" : "wear" ]));
    // Check if he can steal at a 5% chance/heart beat
    SetAction(5, (: Steal :));
    // Don't want him getting cloned all over the place
    SetUnique(1);
    // Set him to wander
    SetWander(25);
    // Any time he hears the word dork, he goes ballistic!
    SetTalkResponses(([ "dork" : "no, YOU are a dork!" ]));
}
