#include <lib.h>
#include <daemons.h>
#include "/lib/include/stargate.h"

/**
 * based on portal.c by Brodbane - March 2006
 *
 * $Id: stargate.c,v 1.1 2006/04/05 05:48:39 jam Exp $
 *
 * The desired functionality is much like a "star gate": users dialed
 * letters or full words that lined up with destinations.  A portal opens to
 * that destination briefly.  To define destinations you must setup a
 * constant below then add it to the switch statement in the cmdDial
 * function.  This object is crude and basic, but gets the job done.
 *
 * 2006-03-22, jonez
 *   - original version of this file is from Daelas@Moraelinost
 * 2006-03-23, jonez
 *   - altered so code uses existing verbs (touch, enter) where possible. last add_action is for dial command.
 *   - added single mapping called "database" and made the "dial" command use it. 
 *   - dial command no longer uses switch/case, making adding a new destination simpler
 *   - made use of SetPreventGet() / SetPreventPut()
 *   - made use of new stargate daemon
 *   - made use of LIB_STARGATE
 *   - made use of STARGATE_D
 *
 * IDEAS:
 *  - create a daemon that holds the stargate network [DONE]
 *  - allow for stargate failure
 *  - add dhd object
 *  - change the code so that it uses a single mapping of names and
 *    destinations, perhaps in a database file. currently an update to the
 *    object requires an update for all the objects. [DONE]
 *  - dhd skill (thanks plato)
 *  - delay when dialing gate. destination dhd lights up?
 *  - player should not be able to dial earth if earth is already connected elsewhere (need daemon) [DONE]
 *  - make use of existing verbs (enter, touch) instead of doing our own thing. [DONE]
 *  - daemon should contain a class that maps the various gates to each other. see lib/include/door.h [DONE]
 *  - shout "off world activation" into the gateroom when the gate engages.
 *  - track status as "incoming" or "outgoing".. you can only "enter" an outgoing gate (rhk) [DONE]
 *  - if room is empty, shut down the gate (rhk)
 *  - change callout time when someone goes through the gate (rhk)
 */

/*
   Cratylus@Dead Souls <ds> but let me give you a quick outline
   Cratylus@Dead Souls <ds> i type dial thing
   Cratylus@Dead Souls <ds> the thing inherits LIB_DIAL
   Cratylus@Dead Souls <ds> which is in /lib/events/dial.c
   Cratylus@Dead Souls <ds> and all that contains is like direct_dial_ob() return 1; this kind of thing
   Cratylus@Dead Souls <ds> take a look at LIB_TURN
   Cratylus@Dead Souls <ds> what LIB_DIAL does is provide the object with hooks, so it *can* be dialed
   Cratylus@Dead Souls <ds> so, when i type dial thing, the thing goes, yeah i
   can be dialed, and then the parser enters the can_dial and do_dial funcs
   in the verb
   Cratylus@Dead Souls <ds> tellya what. i'd like to work with you on this one. i think it's less straightforward than it
   seems, and i need to test some ideas before i can say "do it this way"

 */

#include <this is here on purpose to prevent this from loading>
#include <lib.h>
#include <daemons.h>
#include "/lib/include/stargate.h"

inherit LIB_STARGATE;

int ReadScreen();

void create() 
{
    ::create();
    SetOrigin("ORIGIN_NAME", "/ORIGIN/GOES/HERE");
    SetRead(([ ({ "screen" }) : (: ReadScreen :) ]) );
    SetItems(([ ({ "screen" }) : "a computer screen which shows the status of the gate network" ]) );
}

void init()
{
    ::init();
}

int ReadScreen()
{
    write("stargate network status\n");
    write("-----------------------\n");
    write("\n");

}
