#include <message_class.h>

private static object LastEnvironment = 0;

varargs int eventPrint(string str, mixed args...);

object GetLastEnvironment() {
    return LastEnvironment;
}

int eventMove(mixed dest) {
    object ob,to,furn;
    int depth;
    to=this_object();

    if( !this_object() ) {
	return 0;
    }
    if( environment() ) {
	int x;

	x = (int)environment()->CanRelease(this_object());
	if( !x && !archp() ) {
	    return 0;
	}
    }
    if( stringp(dest) ) {
	if( !(ob = find_object(dest)) ) {
	    string str;

	    if( str = catch(call_other(dest, "???")) ) {
		if( creatorp() ) {
		    eventPrint(str, MSG_ERROR);
		}
		return 0;
	    }
	    ob = find_object(dest);
	}
    }
    else {
	ob = dest;
    }
    if( !ob || ob == this_object() ) {
	return 0;
    }
    if( living(this_object()) && living(ob) ) {
	if(!ob->GetMount()) return 0;
    }
    if( !ob->CanReceive(this_object()) ) {
	return 0;
    }
    if( objectp(to) && LastEnvironment = environment() ) {
	environment()->eventReleaseObject(this_object());
	if(inherits("/lib/std/storage",to) && inherits("/lib/std/storage",environment() ) ) {
	    depth = to->GetRecurseDepth();
	    depth += 1;
	    if(depth) environment()->AddRecurseDepth(-depth);
	    if(environment()->GetRecurseDepth() < 1) environment()->SetRecurseDepth(1);
	}

    }
    if(!objectp(to) ) return 0; 
    move_object(ob);
    environment()->eventReceiveObject(this_object());
    if( environment() ) {
	foreach(object peer in all_inventory(environment())) {
	    if( peer != this_object() ) {
		catch(peer->eventEncounter(this_object()));
	    }
	}
    }

    if(living(this_object()) && furn = this_object()->GetProperty("furniture_object"))
	if(objectp(furn)) furn->eventReleaseStand(this_object());

    return (LastEnvironment != environment());
}
