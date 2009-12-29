/*    /secure/daemon/events.c
 *    from the Dead Souls Library
 *    an event monitoring daemon, for call outs across time
 *    created by Descartes of Borg 950501
 */

#include <lib.h>
#include <logs.h>
#include <save.h>
#include <daemons.h>
#include <privs.h>
#include "events.h"

inherit LIB_DAEMON;

private int RebootInterval;
private mapping Events;
private static int InReboot = 0;
private static int i, callout = -1;
static string SaveFile;
static int *events;

static void create() {
    daemon::create();
    SaveFile = save_file(SAVE_EVENTS);
    if(!file_exists(SaveFile) && file_exists(old_savename(SaveFile))){
        cp(old_savename(SaveFile), SaveFile);
    }
    SetNoClean(1);
    if( file_exists(SaveFile) ){
        RestoreObject(SaveFile);
    }
    if( !RebootInterval ) RebootInterval = 170;
    if( !Events ) Events = ([]);
    eventSave();
    call_out((: eventPollEvents :), 60);
}

mixed eventCancelShutdown() {
    int ret = -3;
#ifdef __CALLOUT_HANDLES__
    if( !(master()->valid_apply( ({ PRIV_ASSIST }) )) ) return -2;
    if(callout > -1) ret = remove_call_out(callout);
    else ret = -1;
    if(!ret || ret == -1) callout = -1;
#endif
    return ret;
}

varargs static int eventSave(int ung) {
    return SaveObject(SaveFile);
}

void DoSaves(){
    object *persists = objects( (: $1->GetPersistent() :) );
    foreach(object persist in persists){
        persist->eventDestruct();
    }
}

int GetRebooting(){
    return InReboot;
}

void eventReboot(int x){
    if( previous_object() && !(master()->valid_apply(({ PRIV_ASSIST })))){
        return;
    }
    InReboot = 1;
    if( x < 1 ){
        x = 1;
    }
    x *= 60;
    message("broadcast", mud_name() + " will reboot in " +
            consolidate(x/60, "a minute") + ".", users());
    if( x < 61 ){
        callout = call_out( (: eventAnnounceReboot, 10 :), x - 10);
    }
    else {
        int y;

        y = x/60;
        y = ((2*y)/3) * 60;
        callout = call_out( (: eventAnnounceReboot($(y)) :), x - y);
    }
}

static void eventAnnounceReboot(int x) {
    if( x == 10 ) {
        message("broadcast", "Last warning: Reboot in 10 seconds.", users());
        call_out( (: DoSaves :), 7 );
        callout = call_out( (: Shutdown :), 10 );
    }
    else if( x < 61 ) {
        message("broadcast", mud_name() + " will reboot in a minute.",
                users());
        callout = call_out( (: eventAnnounceReboot, 10 :), 50);
    }
    else {
        int y;

        message("broadcast", "Reboot in " + (x/60) + " minutes.", users());
        y = ((2 * (x/60))/3) * 60;
        callout = call_out( (: eventAnnounceReboot($(y)) :), x - y);
    }
}

void eventShutdown() { 
    if( !(master()->valid_apply( ({ PRIV_ASSIST }) )) ) return;
    Shutdown();
}

static void Shutdown() {
    message("broadcast", "Shutting down " + mud_name() + " immediately!",
            users());
    map(users(), (: catch($1->cmdQuit()) :));
    shutdown();
}

static void eventPollEvents() {
    int x;
    call_out((: eventPollEvents :), 60);
    x = time();
    i = sizeof(events = keys(Events));
    while(i--) {
        if( events[i] <= x ) {
            object ob;
            function f;
            if(EVENTS_LOGGING){
                unguarded( (: write_file(LOG_EVENTS,timestamp()+" "+
                                Events[events[i]]["object"]+"->"+
                                Events[events[i]]["function"]+
                                "("+implode(Events[events[i]]["args"], ",")+
                    ")\n") :) );
            }
            if( !(ob = find_object(Events[events[i]]["creator"]) )
                    && !(ob = load_object(Events[events[i]]["creator"])) ) {
                map_delete(Events, events[i]);
                continue;
            }
            f = (: call_other, Events[events[i]]["object"],
                    Events[events[i]]["function"] :);
            f = bind(f, ob);
            catch(evaluate(f, Events[events[i]]["args"]...));
            if( Events[events[i]]["regular"] > 0 )
                Events[x + Events[events[i]]["interval"]] = Events[events[i]];
            map_delete(Events, events[i]);
        }
    }
    if( (uptime() > RebootInterval*3600) && !InReboot && !DISABLE_REBOOTS) {
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
    mapping NewEvent;
    if( file_name(previous_object()) != SEFUN && 
            file_name(previous_object()) != UPDATE_D) {
        if(EVENTS_LOGGING){
            unguarded( (: write_file(LOG_EVENTS,timestamp()+" "+
                            identify(previous_object(-1))+
                            " ILLEGALLY tried to add an event.\n") :) );
        }
        return;
    }
    NewEvent = ([ "object" : s, "function" : f, "args" : a,
            "creator" : c,  "regular" : (r ? w : 0), "interval" : w ]);
    if(EVENTS_LOGGING)
        unguarded( (: write_file(LOG_EVENTS,timestamp()+
                        identify(previous_object(-1))+" added this event: "+
                        identify($(NewEvent))+"\n") :) );		
    Events[time() + w] = NewEvent;
    eventSave(1);
}

void RemoveEvent(int x){
    if( file_name(previous_object()) != SEFUN ) return;
    if(sizeof(Events[x])){
        map_delete(Events, x);
        eventSave(1);
    }
}

mapping GetEvents() { return copy(Events); }
