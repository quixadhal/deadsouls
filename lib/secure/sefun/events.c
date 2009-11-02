/*    /secure/sefun/events.c
 *    from the Dead Souls LPC Library
 *    simuls for handling events
 *    created by Descartes of Borg 950514
 */

#include <daemons.h>

varargs void add_event(string source, string ob, string fun, mixed *args, int when, int repeat){
    if(!archp(this_player())) return;
    EVENTS_D->AddEvent(source, ob, fun, args, when, repeat);
}

varargs void remove_event(int i){
    if(!archp(this_player())) return;
    EVENTS_D->RemoveEvent(i);
}

int event_pending(object ob) {
    mapping tmp;
    int *cles;
    string fn;
    int i;

    tmp = EVENTS_D->GetEvents();
    fn = file_name(ob);
    i = sizeof(cles = keys(tmp));
    while(i--) if( tmp[cles[i]]["object"] == fn ) return 1;
    return 0;
}
