/*    /secure/sefun/events.c
 *    from the Dead Souls LPC Library
 *    simuls for handling events
 *    created by Descartes of Borg 950514
 */

#include <daemons.h>

varargs void create_event(string f, int when, mixed *args, int reg) {
    object ob;
    string fn;

    if( clonep(ob = previous_object()) ) return;
    if( !function_exists(f, ob) || when < 60 ) return;
    fn = file_name(ob);
    EVENTS_D->AddEvent(fn, fn, f, when, args, reg);
}

int event_pending(object ob) {
    mapping tmp;
    int *cles;
    string fn;
    int i;

    tmp = (mapping)EVENTS_D->GetEvents();
    fn = file_name(ob);
    i = sizeof(cles = keys(tmp));
    while(i--) if( tmp[cles[i]]["object"] == fn ) return 1;
    return 0;
}
