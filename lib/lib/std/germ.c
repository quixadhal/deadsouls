/*    /lib/germ.c
 *    From the Dead Souls Object Library
 *    Germs which cause disease
 *    created by Descartes of Borg 940905
 *    Version: @(#) germ.c 1.12@(#)
 *    Last modified: 97/01/01
 */

#include <lib.h>

inherit LIB_ITEM;

private int             Communicable  = 1;
private static mixed    Cure          = 0;
private static function Infect        = 0;
private static int      LastHeartBeat = time();
private int             LifeSpan      = 60;
private string          Type          = "cold";

mixed eventMultiply();

/* ***************  /lib/germ.c data functions  *************** */
int isGerm() {
    return 1;
}

int GetCommunicable() {
    return Communicable;
}

/*
 * int SetCommunicable(int x);
 * int x - how infectious the germ is
 *
 * description:
 * Makes the germ multiply and infect on its own.  The more
 * communicable, the easier it infects hosts and the easier
 * it multiplies.
 * Non-communicable germs must be forced to infect hosts
 *
 * returns the degree to which it is communicable
 */
int SetCommunicable(int x) {
    if( x > 50 ) x = 50;
    return (Communicable = x);
}

mixed GetCure() {
    return Cure;
}

/* mixed SetCure(mixed val) 
 * mixed val - function to test whether germ is cured 
 *             or int indicating how hard it is to cure
 *             -1 means incurable
 *
 * description
 * If a function, then that function will be used to 
 * determine if a given cure attempt is successful.
 * That function should return 1 for success, 0 for failure
 * If an int and -1, the germ is incurable
 * If an int > 0, then that much of a cure of the right
 * type is needed to cure the disease
 *
 * returns the successfully set value
 */
mixed SetCure(mixed val) {
    return (Cure = val);
}

string array GetId() {
    return ({ item::GetId()..., "germ" });
}

function GetInfect() {
    return Infect;
}

/* function SetInfect(function f)
 * function f - Function to be called whenever this germ
 *              infects a host
 *
 * Sets a function to be called whenever a host is infected
 *
 * returns the function to be called
 */
function SetInfect(function f) {
    return (Infect = f);
}

int GetLifeSpan() {
    return LifeSpan;
}

/* int SetLifeSpan(int x)
 * int x - the length of time the germ can live outside
 *         a host
 * 
 * description
 * Sets the amount of time that can pass for the germ
 * to exist without a host
 *
 * returns the new life span
 */
int SetLifeSpan(int x) {
    if( x > 600 ) x = 600;
    return (LifeSpan = x);
}

string GetType() {
    return Type;
}

/* string SetType(string type)
 * string type - the nature of the infection
 *
 * description
 * determines what sort of cure is needed
 * valid values are:
 *     viral
 *     bacterial
 *
 * returns the new germ type
 */
string SetType(string type) {
    return (Type = type);
}

/* ***************  /lib/germ.c events  *************** */
/**
 * This event is triggered from within the player object by some
 * attempt by a cleric or other some such thing to cure the player.
 */
mixed eventCure(object who, int x, string type) {
    if( type != GetType() ) { // Can't cure this
	return 0;
    }
    if( Cure == -1 ) { // This is incureable
	return 0;
    }
    if( functionp(Cure) ) {
	mixed tmp = evaluate(Cure, who, x, type);
	
	if( tmp != 1 ) {
	    return tmp;
	}
	Cure = 0;
    }
    else if( intp(Cure) ) {
	Cure -= x;
    }
    else {
	error("Bad argument 2 to eventCure().");
    }
    if( Cure > 0 ) {
	return 0;
    }
    set_heart_beat(0);
    Destruct();
    return 1;
}

mixed eventEncounter(object who) {
    if( !living(who) ) {
	return 1;
    }
    if( !query_heart_beat() ) {
	eventMultiply();
	set_heart_beat(5);
    }
    return 1;
}

mixed eventInfect(object ob) {
    mixed tmp;
    
    if( functionp(Infect) ) {
	tmp = evaluate(Infect, ob);
	if( tmp != 1 ) {
	    Destruct();
	    return tmp;
	}
    }
    if( !query_heart_beat() ) {
	set_heart_beat(5);
    }
    eventMove(ob);
    return 1;
}

mixed eventMultiply() {
    object ob;

    if( Communicable > random(3000) ) {
	object array germs = filter(deep_inventory(environment()),
				    (: $1->isGerm() :));
	object germ;

	if( sizeof(germs) > 5 ) {
	    return 0;
	}
	germ = new(base_name(this_object()));
	if( !germ ) {
	    return 0;
	}
        if( (ob = environment()) && random(100) > 50 ) {
	    ob = (environment(ob) || ob);
	}
	if( living(ob) ) {
	    if( ob->eventInfect(germ) != 1 ) {
		germ->eventDestruct();
	    }
	}
	else {
	    germ->eventMove(ob);
	}
        return 1;
    }
    return 0;
}

void eventSuffer(object ob) {
}
   
/* ***************  /lib/germ.c driver applies  *************** */

static void create() {
    AddSave(({ "Communicable", "LifeSpan", "Type" }));
    item::create();
    SetInvis(1);
    SetMass(0);
    SetValue(0);
    SetPreventDrop("");
    SetPreventGet("");
    SetPreventPut("");
    call_out(function() { // Start up the HB if cloned into living thing
	         object env = environment();

		 if( !env || !living(env) ) {
		     return;
		 }
		 set_heart_beat(5);
             }, 2);
}

static void heart_beat() {
    object array victims;
    object env = environment();
    int interval;

    interval = time() - LastHeartBeat;
    LastHeartBeat = time();
    if( !env ) {
	set_heart_beat(0);
	if( clonep() ) {
	    Destruct();
	}
	return;
    }
    if( living(env) && environment(env) ) {
        eventSuffer(env);
	if( !this_object() ) {
	    return;
	}
        if( !env ) {
	    if( this_object() ) {
		Destruct();
	    }
            return;
        }
        if( Communicable ) {
	    eventMultiply();
	}
        victims = filter(all_inventory(environment(env)),
			 (: living :)) - ({ env });
    }
    else {
        LifeSpan -= interval;
        if( LifeSpan < 1 ) {
            Destruct();
            return;
        }
        victims = filter(all_inventory(env), (: living :));
        if( !sizeof(victims) ) {
            set_heart_beat(0);
            return;
        }
        if( Communicable ) {
	    eventMultiply();
	}
    }
    if( Communicable > random(1000) ) {
	object target, germ;
	
	victims = sort_array(victims,
			     function(object one, object two) {
	                         if( one->GetHealthPoints() >
				     two->GetHealthPoints() ) {
				     return -1;
				 }
				 else {
				     return 1;
				 }
	                     });
        if( sizeof(victims) ) {
	    target = victims[0];
	}
        else {
	    return;
	}
	germ = new(base_name(this_object()));
	if( germ ) {
	    if( target->eventInfect(germ) != 1 ) {
		germ->eventDestruct();
	    }
	}
    }
}
