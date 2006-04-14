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

private mapping stargates = ([]);

static void create()
{
    daemon::create();
    SetNoClean(1);
    load();
    if (!stargates) stargates = ([]);
}

void save()
{
    unguarded( (: save_object, SAVE_STARGATE, 1 :) );
    tell_player("jonez", "stargate daemon saved itself");
    return;
}

void load()
{
    if (file_size(SAVE_STARGATE __SAVE_EXTENSION__) > 0)
    {
	tc("stargate save file exists");
	unguarded( (: restore_object, SAVE_STARGATE :) );
    }
    tell_player("jonez", "stargate daemon loaded itself");
    return;
}

int setStargate(string address, string destination)
{
    class stargate s = new(class stargate);

    if (address == "" || destination == "") return 1;

    s->status = "idle";
    s->destination = destination;
    s->endpoint = 0;
    stargates[address] = s;
    save();
    return 0;
}

class stargate getStargate(string address)
{
    class stargate s = stargates[address];
    if (!s) return 0;

    tell_player("jonez", sprintf("get: address=%s, status=%s destination=%s endpoint=%s", address, s->status, s->destination, s->endpoint));
    return (class stargate)stargates[address];
}

int delStargate(string address)
{
    map_delete(stargates, address);
    save();
    return 0;
}

mapping getStargates()
{
    /*
      string buf = "";

      foreach(string n, class stargate g in stargates)  
      {
	buf += sprintf("name=%s, status=%s, destination=%s, endpoint=%s; ", n, g->status, g->destination, g->endpoint);
      }
      return buf;
    */
    return copy(stargates);
}

int setStatus(string address, string status)
{
    class stargate s;
    s = stargates[address];
    if (!s) return 1;

    s->status = status;
    save();
    return 0;
}

string getStatus(string address)
{
    class stargate s = stargates[address];
    if (!s) return "";

    return s->status;
}

string getDestination(string address)
{
    class stargate s = stargates[address];
    if (!s) return "";
    return s->destination;
}

string getEndpoint(string address)
{
    class stargate s = stargates[address];
    if (!s) return "";
    return s->endpoint;
}

// return 1 on success, 0 on error
int connect(string from, string to)
{
    class stargate t, f;

    tell_player("jonez", sprintf("stargate_d->connect(%s, %s)", from, to));

    if (from == to) return 0;

    f = stargates[from];
    if (!f) 
    {
	tell_player("jonez", "failed to lookup status of outbound gate");
	return 0;
    }

    t = stargates[to];
    if (!t) 
    {
	tell_player("jonez", "failed to lookup status of inbound gate");
	return 0;
    }

    tell_player("jonez", sprintf("%s->status=%s, %s->status=%s", from, f->status, to, t->status));

    if (f->status == "idle" && t->status == "idle")
    {
	f->endpoint = to;
	f->status = "outbound";

	t->endpoint = from;
	t->status = "inbound";

	save();

	return 1;
    }
    return 0;
}

// return 1 on success, 0 on error
int disconnect(string from)
{
    class stargate f;
    string endpoint;

    tell_player("jonez", sprintf("stargate_d->disconnect(%s)", from));

    f = stargates[from];
    if (!f) return 0;

    endpoint = stargates[from]->endpoint;
    if (!endpoint) return 0;

    stargates[endpoint]->endpoint = "";
    stargates[endpoint]->status = "idle";

    stargates[from]->endpoint = "";
    stargates[from]->status = "idle";
    save();
    return 1;
}
