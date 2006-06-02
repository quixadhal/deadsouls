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
    //tc("stargate daemon saved itself");
    return;
}

void eventLoad(){
    if (file_size(SAVE_STARGATE __SAVE_EXTENSION__) > 0){
	//tc("stargate save file exists");
	unguarded( (: restore_object, SAVE_STARGATE :) );
    }
    //tc("stargate daemon loaded itself");
    //tc("Stargates: "+identify(Stargates),"red");
    return;
}

int SetStargate(string address, string destination){
    mapping tmp = ([]);
    Stargates[address] = tmp;
    //tc("Stargates: "+identify(Stargates),"yellow");
    if (address == "" || destination == "") return 1;
    if(sizeof(Stargates[address])) return 1;
    Stargates[address]["status"] = "idle";
    Stargates[address]["destination"] = destination;
    Stargates[address]["endpoint"] = "";
    eventSave();
    return 0;
}

mapping GetStargate(string address){
    //tc("Stargates: "+identify(Stargates),"blue");
    return copy(Stargates[address]);
}

int RemoveStargate(string address){
    //tc("Stargates: "+identify(Stargates),"red");
    map_delete(Stargates, address);
    //tc("Stargates: "+identify(Stargates),"cyan");
    eventSave();
    return 0;
}

mapping GetStargates(){
    //tc("Stargates: "+identify(Stargates),"blue");
    return copy(Stargates);
}

int SetStatus(string address, string status){
    //tc("Setting status of "+status+" on "+address+".");
    Stargates[address]["status"] = status;
    eventSave();
    return 0;
}

string GetStatus(string address){
    //tc("Stargates: "+identify(Stargates),"red");
    return Stargates[address]["status"];
}

string GetDestination(string address){
    string ret = Stargates[address];
    //tc("Stargates: "+identify(Stargates),"white");
    if(sizeof(Stargates[address]) && sizeof(Stargates[address]["destination"]))
	return Stargates[address]["destination"];
    else return "";
}

string GetEndpoint(string address){
    //tc("Stargates: "+identify(Stargates),"blue");
    return Stargates[address]["endpoint"];
}

int eventConnect(string from, string to){
    //tc("Stargates: "+identify(Stargates),"red");
    //tc("STARGATE_D, from: "+from+", to: "+to,"green");

    if (from == to) return 0;

    if (!Stargates[from] || !sizeof(Stargates[from])){ 
	//tc("failed to lookup status of outbound gate");
	return 0;
    }

    if (!Stargates[to] || !sizeof(Stargates[to])){ 
	//tc("failed to lookup status of inbound gate");
	return 0;
    }

    if (Stargates[from]["status"] == "idle" && Stargates[to]["status"] == "idle"){
	//tc("situation normal");

	Stargates[from]["endpoint"] = to;
	Stargates[from]["status"] = "outbound";

	Stargates[to]["endpoint"] = from;
	Stargates[to]["status"] = "inbound";
	//tc("Stargates: "+identify(Stargates));
	eventSave();

	return 1;
    }
    //tc("exception");
    return 0;
}

int eventDisconnect(string from){
    string endpoint;
    if(!from || from == "") return 0;
    //tc("Stargates: "+identify(Stargates),"blue");

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
