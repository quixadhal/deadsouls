/*    /secure/sefun/communications.c
 *    from the Dead Souls Object Library
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

void tell_player(mixed player, string msg){
    object dude;
    string str;
    if(objectp(player)) str = player->GetKeyName();
    else str = player;
    if(!msg || msg == "") return;
    if(!dude = find_player(str) ) return;
    else tell_object(dude, msg);
}

varargs void tc(string str, string col){
    string prefix;
    if(!col) col = "magenta";

    switch(col){
    case "red" : prefix = "%^BOLD%^RED%^";break;
    case "cyan" : prefix = "%^BOLD%^CYAN%^";break;
    case "blue" : prefix = "%^BOLD%^BLUE%^";break;
    case "yellow" : prefix = "%^BOLD%^YELLOW%^";break;
    case "green" : prefix = "%^BOLD%^GREEN%^";break;
    default : prefix = "%^BOLD%^MAGENTA%^";break;
    }

    tell_player("cratylus",prefix+str+"%^RESET%^");
}

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

