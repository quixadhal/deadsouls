/**
 * stargate_d : stargate daemon. tracks the entire stargate network.
 * started 2006-03-23 by jonez
 *
 * 2006-03-24, jonez
 *  - with help from cratylus, changed the daemon so it saves it's state
 *    whenever there is a change, and tries to load from disk at create
 *    time.
 * 2006-03-28, jonez
 *  - gate is status is now "idle", "outbound", or "inbound". gate cannot be entered from the inbound side.
 */

#include <lib.h>
#include <save.h>
#include "/daemon/include/stargate.h"

inherit LIB_DAEMON;

private mapping Stargates = ([]);

static void create(){
    daemon::create();
    SetNoClean(1);
    eventLoad();
    if (!Stargates) Stargates = ([]);
}

void eventSave(){
    unguarded( (: save_object, SAVE_STARGATE, 1 :) );
    return;
}

void eventLoad(){
    if (file_size(SAVE_STARGATE __SAVE_EXTENSION__) > 0){
        unguarded( (: restore_object, SAVE_STARGATE :) );
    }
    return;
}

int SetStargate(string address, string destination){
    mapping tmp = ([]);
    Stargates[address] = tmp;
    if (address == "" || destination == "") return 1;
    if(sizeof(Stargates[address])) return 1;
    Stargates[address]["status"] = "idle";
    Stargates[address]["destination"] = destination;
    Stargates[address]["endpoint"] = "";
    eventSave();
    return 0;
}

mapping GetStargate(string address){
    return copy(Stargates[address]);
}

int RemoveStargate(string address){
    map_delete(Stargates, address);
    eventSave();
    return 0;
}

mapping GetStargates(){
    return copy(Stargates);
}

int SetStatus(string address, string status){
    Stargates[address]["status"] = status;
    eventSave();
    return 0;
}

string GetStatus(string address){
    return Stargates[address]["status"];
}

string GetDestination(string address){
    if(sizeof(Stargates[address]) && sizeof(Stargates[address]["destination"]))
        return Stargates[address]["destination"];
    else return "";
}

string GetEndpoint(string address){
    return Stargates[address]["endpoint"];
}

int eventConnect(string from, string to){

    if (from == to) return 0;

    if (!Stargates[from] || !sizeof(Stargates[from])){ 
        return 0;
    }

    if (!Stargates[to] || !sizeof(Stargates[to])){ 
        return 0;
    }

    if (Stargates[from]["status"] == "idle" && Stargates[to]["status"] == "idle"){

        Stargates[from]["endpoint"] = to;
        Stargates[from]["status"] = "outbound";

        Stargates[to]["endpoint"] = from;
        Stargates[to]["status"] = "inbound";
        eventSave();

        return 1;
    }
    return 0;
}

int eventDisconnect(string from){
    string endpoint;
    if(!from || from == "") return 0;

    if (!Stargates[from] || !sizeof(Stargates[from])) return 0;

    endpoint = Stargates[from]["endpoint"];
    if (!endpoint) return 0;

    if(sizeof(Stargates[endpoint])){
        Stargates[endpoint]["endpoint"] = "";
        Stargates[endpoint]["status"] = "idle";
    }

    Stargates[from]["endpoint"] = "";
    Stargates[from]["status"] = "idle";

    eventSave();
    return 1;
}
