/*    /secure/sefun/communications.c
 *    from the Dead Souls Object Library
 *    some backwards compat sefuns
 */

#include <message_class.h>
#include <config.h>
#include <rooms.h>

object *global_tmp_ob_arr;

varargs void say(mixed str, mixed ob) {
    object *obs;

    if(!this_player()) error("say() makes no sense with no this_player()\n");
    if(!environment(this_player())) return;
    if( !ob ) obs = ({ this_player() });
    else if( objectp(ob) ) obs = ({ ob, this_player() });
    else obs = ob + ({ this_player() });
    environment(this_player())->eventPrint(str + "", MSG_ENV, obs);
}

varargs void tell_object(object ob, mixed str, int mclass){ 
    ob->eventPrint(str, (mclass || MSG_CONV)); 
}

void tell_player(mixed player, string msg){
    object dude;
    string str;
    if(objectp(player)) str = player->GetKeyName();
    else str = player;
    if(!msg || msg == "") return;
    if(!dude = find_player(str) ) return;
    else tell_object(dude, msg);
}

varargs void tc(string str, string col, object dude){
    string prefix;
    if(!col) col = "magenta";
    switch(col){
    case "red" : prefix = "%^BOLD%^RED%^";break;
    case "cyan" : prefix = "%^BOLD%^CYAN%^";break;
    case "blue" : prefix = "%^BOLD%^BLUE%^";break;
    case "yellow" : prefix = "%^BOLD%^YELLOW%^";break;
    case "green" : prefix = "%^BOLD%^GREEN%^";break;
    case "white" : prefix = "%^BOLD%^WHITE%^";break;
    default : prefix = "%^BOLD%^MAGENTA%^";break;
    }
    if(!dude) dude = find_player(DEBUGGER);
    if(dude){
        tell_player(dude ,prefix+str+"%^RESET%^");
        if(dude) flush_messages(dude);
    }
    debug_message(str);
}

varargs int tn(string str, string col, object room, int mclass){
    string prefix;
    if(!col) col = "magenta";
    switch(col){
    case "red" : prefix = "%^BOLD%^RED%^";break;
    case "cyan" : prefix = "%^BOLD%^CYAN%^";break;
    case "blue" : prefix = "%^BOLD%^BLUE%^";break;
    case "yellow" : prefix = "%^BOLD%^YELLOW%^";break;
    case "green" : prefix = "%^BOLD%^GREEN%^";break;
    case "white" : prefix = "%^BOLD%^WHITE%^";break;
    default : prefix = "%^BOLD%^MAGENTA%^";break;
    }
    if(!room) tell_object(load_object(ROOM_NETWORK) ,prefix+str+"%^RESET%^");
    else {
        if(!mclass) tell_object(room, prefix+str+"%^RESET%^");
        else tell_object(room, prefix+str+"%^RESET%^",mclass);
    }
    return 1;
}

varargs int trr(string str, string col, int mclass){
    if(!mclass) mclass = 0;
    tn(str, col, load_object(ROOM_ROUTER), mclass);
    return 1;
}

varargs int debug(mixed msg, mixed val, string color){
    object *players = filter(users(), (: $1->GetProperty("debug") :) );
    string ret = "";
    string prevob = "";
    if(!sizeof(players)) return 0;
    prevob = file_name(previous_object());
    if(msg && stringp(msg) && val) ret += msg;
    else if(msg && !val) val = msg;
    ret += " ";
    if(val) ret += identify(val);
    if(!color || !sizeof(color)) color = "green";
    foreach(object guy in players){
        tc("%^BOLD%^WHITE%^DEBUG: %^RESET%^ "+prevob,color,guy);
        tc(ret, color, guy);
    }
    return 1;
}

varargs int tell_creators(string msg, string color){
    object *cres = filter(users(), (: creatorp($1) :) );
    global_tmp_ob_arr = ({});
    if(!sizeof(cres)) return 0;
    global_tmp_ob_arr = sort_array(cres, (: member_array($1,global_tmp_ob_arr) == -1 :) );
    cres = global_tmp_ob_arr;
    if(!msg) msg = "";
    if(!color) color = "red";
    foreach(object guy in cres){
        tc(msg, color, guy);
    }
    return 1;
}

varargs void tell_room(mixed ob, mixed str, mixed exclude) {
    if(!ob ) return;
    if(stringp(ob) && unguarded( (: !file_exists($(ob)) && !file_exists($(ob)+".c") :) )) return;
    if(stringp(ob) &&!(ob = load_object(ob))) return;
    ob->eventPrint(str, MSG_ENV, exclude);
}

varargs void shout(mixed str, mixed exclude) {
    if(objectp(exclude)) exclude = ({ exclude });
    else if(!pointerp(exclude)) exclude = ({});
    if(this_player()) exclude += ({ this_player() });
    users()->eventPrint(str + "", MSG_CONV, exclude);
}
