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
static string SaveFile;

static void create(){
    daemon::create();
    SaveFile = save_file(SAVE_STARGATE);
    SetNoClean(1);
    eventLoad();
    set_heart_beat(60);
    if (!Stargates) Stargates = ([]);
    SaveObject(SaveFile, 1);
}

void eventSave(){
    SaveObject(SaveFile, 1);
    return;
}

void eventLoad(){
    if(file_exists(SaveFile)){
        RestoreObject(SaveFile);
    }
    return;
}

int SetStargate(string address, string destination){
    mapping tmp = ([]);
    object prevob = previous_object();
    string prev;

    if(!prevob) return 0;
    prev = base_name(prevob);

    //Only gates installed in /domains/ or /secure/ are allowed
    //to register.
    if(strsrch(prev, "/domains/") && strsrch(prev, "/secure/")){
        return 0;
    }

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
    if(!Stargates[address]) return 0;
    Stargates[address]["status"] = status;
    Stargates[address]["last_change"] = time();
    eventSave();
    return 1;
}

string GetStatus(string address){
    if(!Stargates || !Stargates[address]) return "unknown";
    return Stargates[address]["status"];
}

mixed GetDestination(string address){
    if(sizeof(Stargates[address]) && sizeof(Stargates[address]["destination"]))
        return Stargates[address]["destination"];
    else return 0;
}

mixed GetEndpoint(string address){
    if(Stargates[address] && Stargates[address]["endpoint"]){
        return Stargates[address]["endpoint"];
    }
    else return 0;
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

void ResetGates(){
    foreach(mixed key, mixed val in Stargates){
        Stargates[key]["endpoint"] = "";
        Stargates[key]["status"] = "idle";
        Stargates[key]["last_change"] = 0;
        eventSave();
    }
}

void heart_beat(){
    foreach(mixed key, mixed val in Stargates){
        if(undefinedp(val["last_change"])){
            Stargates[key]["last_change"] = 0;
        }
        if((val["last_change"] && (time() - val["last_change"]) > 60) ||
                (!val["last_change"] && val["status"] != "idle")){
            Stargates[key]["endpoint"] = "";
            Stargates[key]["status"] = "idle";
            Stargates[key]["last_change"] = 0;
            eventSave();
        }
    }
}
