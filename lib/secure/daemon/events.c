/*    /secure/daemon/events.c
 *    from the Foundation II LPC Library
 *    an event monitoring daemon, for call outs across time
 *    created by Descartes of Borg 950501
 */

#include <lib.h>
#include <save.h>
#include <config.h>
#include <privs.h>
#include "events.h"

inherit LIB_DAEMON;

private int RebootInterval;
private mapping Events;
private static int InReboot = 0;

static void create() {
    daemon::create();
    SetNoClean(1);
    if( file_exists(SAVE_EVENTS __SAVE_EXTENSION__) )
      unguarded((: restore_object, SAVE_EVENTS :));
    if( !RebootInterval ) RebootInterval = 24;
    if( !Events ) Events = ([]);
    eventSave();
    call_out((: eventPollEvents :), 60);
}

varargs static int eventSave(int ung) {
    if( ung ) {
	unguarded( (: save_object, SAVE_EVENTS :) );
	return 1;
    }
    else return save_object(SAVE_EVENTS);
}

void eventReboot(int x) {
    if( previous_object() && !((int)master()->valid_apply(({ PRIV_ASSIST }))) )
      return;
    if( x < 1 ) x = 1;
    x *= 60;
    message("broadcast", mud_name() + " will reboot in " +
	    consolidate(x/60, "a minute") + ".", users());
    if( x < 61 ) call_out( (: eventAnnounceReboot, 10 :), x - 10);
    else {
	int y;

	y = x/60;
	y = ((2*y)/3) * 60;
	call_out( (: eventAnnounceReboot($(y)) :), x - y);
    }
}

static void eventAnnounceReboot(int x) {
    if( x == 10 ) {
	message("broadcast", "Last warning: Reboot in 10 seconds.", users());
	call_out( (: Shutdown :), 10 );
    }
    else if( x < 61 ) {
	message("broadcast", mud_name() + " will reboot in a minute.",
		users());
	call_out( (: eventAnnounceReboot, 10 :), 50);
    }
    else {
	int y;

	message("broadcast", "Reboot in " + (x/60) + " minutes.", users());
	y = ((2 * (x/60))/3) * 60;
	call_out( (: eventAnnounceReboot($(y)) :), x - y);
    }
}

void eventShutdown() { 
    if( !((int)master()->valid_apply( ({ PRIV_ASSIST }) )) ) return;
    Shutdown();
}

static void Shutdown() {
    message("broadcast", "Shutting down " + mud_name() + " immediately!",
	    users());
    map(users(), (: catch($1->cmdQuit()) :));
    shutdown();
}

static void eventPollEvents() {
    int *events;
    int i, x;

    call_out((: eventPollEvents :), 60);
    x = time();
    i = sizeof(events = keys(Events));
    while(i--) {
        if( events[i] <= x ) {
	    object ob;
	    function f;

	    if( !(ob = load_object(Events[events[i]]["creator"])) ) {
		map_delete(Events, events[i]);
		continue;
	    }
	    f = (: call_other, Events[events[i]]["object"],
		 Events[events[i]]["func"] :);
	    f = bind(f, ob);
            catch(evaluate(f, Events[events[i]]["args"]...));
            if( Events[events[i]]["regular"] > 3599 )
              Events[x + Events[events[i]]["regular"]] = Events[events[i]];
	    map_delete(Events, events[i]);
        }
    }
    if( (uptime() > RebootInterval*3600) && !InReboot ) {
	InReboot = 1;
        eventReboot(MINUTES_REBOOT_WARNING);
    }
    eventSave();
}

int SetRebootInterval(int x) {
    int y;

    y = RebootInterval;
    if( x > 1 ) RebootInterval = x;
    if( !eventSave() ) RebootInterval = y;
    return RebootInterval;
}

int GetRebootInterval() { return RebootInterval; }

void AddEvent(string c, string s, string f, mixed *a, int w, int r) {
    if( file_name(previous_object()) != SEFUN ) return;
    Events[time() + w] = ([ "object" : s, "function" : f, "args" : a,
			      "creator" : c,  "regular" : (r ? w : 0) ]);
    eventSave(1);
}

mapping GetEvents() { return copy(Events); }


