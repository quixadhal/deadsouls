/*    /secure/sefun/communications.c
 *    from the Dead Soulsr2 Object Library
 *    some backwards compat sefuns
 */

#include <message_class.h>

varargs void say(mixed str, mixed ob) {
    object *obs;
    
    if(!this_player()) error("say() makes no sense with no this_player()\n");
    if(!environment(this_player())) return;
    if( !ob ) obs = ({ this_player() });
    else if( objectp(ob) ) obs = ({ ob, this_player() });
    else obs = ob + ({ this_player() });
    environment(this_player())->eventPrint(str + "", MSG_ENV, obs);
}

void tell_object(object ob, mixed str) { ob->eventPrint(str, MSG_CONV); }

varargs void tell_room(object ob, mixed str, mixed exclude) {
    if(!ob) return;
    ob->eventPrint(str, MSG_ENV, exclude);
}

varargs void shout(mixed str, mixed exclude) {
    if(objectp(exclude)) exclude = ({ exclude });
    else if(!pointerp(exclude)) exclude = ({});
    if(this_player()) exclude += ({ this_player() });
    users()->eventPrint(str + "", MSG_CONV, exclude);
}

