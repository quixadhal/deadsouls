#include <lib.h>
#include <daemons.h>
#include "/lib/include/stargate.h"

/**
 * 2006-03-28, jonez
 *  - based on a suggestion from rhk, changed so that one cannot enter the
 *    gate unless status is "outbound". this makes the object closer to the
 *    (theoretical) math and also to what happens on the show.
 */

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

inherit LIB_ITEM;

static private string origin;
static private int connect_time;

string displayLong();
string displayShort();

void create(){ 
    item::create();

    SetKeyName("stargate");
    SetId(({"stargate", "gate", "gateway", "ring"}));
    SetAdjectives(({"stargate"}));
    SetShort( (: displayShort :) );

    SetLong( (: displayLong :) );

    AddItem( "inner ring", "The second ring - the inner ring - is placed "
            "inside the larger ring and seems to be able to move.");
    SetMass(1000);
    SetBaseCost("silver",50);
    SetPreventGet("The gate is pure naquadah and cannot be moved.");
    SetPreventPut("The gate is pure naquadah and cannot be moved!");
    SetTouch("You feel the stargate beneath your hand humming with energy.");
}

void init(){
    ::init();
    add_action( "cmdDial", "dial" );
    add_action( "cmdEnter", "enter");
}


void SetOrigin(string o, string d){
    if (o == "" || d == "") return;    
    origin = lower_case(o);
    if(!sizeof(STARGATE_D->GetStargate(origin))){
        STARGATE_D->SetStargate(origin, d);
    }
}

string GetOrigin(){
    return origin;
}

void eventConnect(string destination){
    int ret;

    destination = lower_case(destination);

    if (origin == destination){
        write("You attempt to dial the gate, but the last chevron does not engage");
        say(this_player()->GetName() + " tries to dial the gate but the last chevron does not engage");
        return;
    }

    ret = STARGATE_D->eventConnect(origin, destination);
    if (ret){
        string d = STARGATE_D->GetDestination(destination);
        write("The ancient rings lock into place and a gateway forms in an explosion of energy.");
        say("The ancient rings lock into place and a gateway forms in an explosion of energy.");
        tell_room(d, "The ancient rings lock into place and a gateway forms in an explosion of energy");
        call_out("eventDisconnect", 10+random(5));
        connect_time = time();
        return;
    }

    write("You attempt to dial the stargate, but nothing happens.");
    say(this_player()->GetName() + " tries to dial the gate but fails.");

    return;
}

int eventDisconnect(){
    string endpoint = STARGATE_D->GetEndpoint(origin);
    string e = STARGATE_D->GetDestination(origin);
    string d = STARGATE_D->GetDestination(endpoint);
    if(d) tell_room(d, "The chevrons on the stargate disengage and the gateway disappears.");
    if(e) tell_room(e, "The chevrons on the stargate disengage and the gateway disappears.");
    connect_time = 0;
    return STARGATE_D->eventDisconnect(origin);
}

string status(){
    return STARGATE_D->GetStatus(origin);  
}

mixed cmdDial(string s){
    object ob;
    string flipside;
    if (s)
    {
        if(STARGATE_D->GetDestination(s)) 
            flipside = STARGATE_D->GetDestination(s);
        eventConnect(s);
        if(sizeof(flipside) && !ob) ob = load_object(flipside);
        if(!ob){
            write("The Stargate abruptly begins to shuts down.");
            eventDisconnect();
        }
        return 1;
    }

    return 0;

}

int cmdEnter(string what){
    string endpoint, destination;
    object who;

    if(!answers_to(what, this_object()))
    {
        return 0;
    }

    if (status() != "outbound")
    {
        return 0;
    }

    who = this_player();
    endpoint = STARGATE_D->GetEndpoint(origin);
    destination = STARGATE_D->GetDestination(endpoint);
    who->eventPrint("You step through the event horizon of the stargate.");
    who->eventMoveLiving(destination, 
            "$N enters into the event horizon and disappears.", 
            "$N emerges from the event horizon.");
    return 1;
}

int eventEnter(object who){
    string endpoint;

    if (!who) return 0;

    endpoint = STARGATE_D->GetEndpoint(origin);
    if (status() == "connected")
    {
        who->eventPrint("You step through the event horizon of the stargate.");
        who->eventMoveLiving(endpoint, 
                "$N enters into the event horizon and disappears.", 
                "$N emerges from the event horizon.");
    }
    return 1;
}

string displayLong(){
    string buf, stat;

    buf = "This is the Stargate of legend.  The Stargate was created "
        "from naquadah ore, similar to black quartz.  It is a perfectly "
        "circular device approximately ten meters in diameter and "
        "comprised of two sets of rings and nine chevrons placed "
        "equidistant along its outer circumference.";

    stat = status();

    if (stat == "outbound" || stat == "inbound")
    {
        buf += " There is an event horizon in the center of the ring that looks like shimmering water.";
    }
    else if (stat == "idle")
    {
        buf += " This gate is currently idle.";
    }
    return buf;
}

string displayShort(){
    string stat;
    stat = status();
    switch (stat)
    {
        case "inbound":
            return "an inbound stargate";
        case "outbound":
            return "an outbound stargate";
        case "idle":
            return "an idle stargate";
        default:
            return "a broken stargate";
    }
}

void heart_beat(){
    if(connect_time && (time() - connect_time ) > 60){
        eventDisconnect();
    }
}
