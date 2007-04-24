#include <message_class.h>
#include <medium.h>

private static object LastEnvironment = 0;

varargs int eventPrint(string str, mixed args...);
varargs void eventAnnounceCanonicalId(object env);

object GetLastEnvironment() {
    return LastEnvironment;
}

int eventMove(mixed dest) {
    object ob,to,furn,prev;
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
    prev = environment(ob);
    move_object(ob);
    ob->eventReceiveObject(this_object());
    if(environment() == prev) return 0;
    if( environment() ) {
	foreach(object peer in all_inventory(environment())) {
	    if( peer != this_object() ) {
		catch(peer->eventEncounter(this_object()));
	    }
	}
	if(OBJECT_MATCHING){ 
	    object *prev_inv = ({});
	    eventAnnounceCanonicalId();
	    //tc("I am "+identify(this_object())+" and I am here: "+identify(environment()),"yellow");
#if 1
	    if(prev && sizeof(prev_inv = deep_inventory(prev)))
		prev_inv->ReceiveCanonicalId(ob->GetCanonicalId(), 1);
#endif
	}
    }

    if(living(this_object()) && furn = this_object()->GetProperty("furniture_object"))
	if(objectp(furn)) furn->eventReleaseStand(this_object());

    if(living(this_object()) && !living(environment(this_object())) 
      && furn = this_object()->GetProperty("mount"))
	if(objectp(furn)) {
	    this_object()->RemoveProperty("mount");
	    furn->eventDismount(this_object());
	}
    if(environment()->GetMedium() == MEDIUM_AIR){
	if(!(this_object()->CanFly())) call_out("eventFall", 1);
	else this_object()->eventFly();
    }
    return (LastEnvironment != environment());
}
