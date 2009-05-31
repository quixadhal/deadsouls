/*    /secure/sefun/communications.c
 *    from the Dead Souls Mud Library
 *    some backwards compat sefuns
 */

#include <message_class.h>
#include ROOMS_H

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
    if(ob) ob->eventPrint(str, (mclass || MSG_CONV)); 
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

static string tc_color_logic(string col){
    string ret;
    if(!col) col = "magenta";
    ret = "%^BOLD%^"+upper_case(col)+"%^";
    if(!grepp(ret, "B_")){
        if(grepp(ret, "WHITE")) ret = "%^B_BLACK"+ret;
        else if(grepp(ret, "BLACK")) ret = "%^B_WHITE"+ret;
    }
    return ret;
}

varargs void tc(string str, string col, object dude){
    string prefix;
    if(!col) col = "magenta";
    prefix = tc_color_logic(col);
    if(!dude) dude = find_player(DEBUGGER);
    if(dude){
        tell_player(dude, prefix+str+"%^RESET%^");
        if(dude) flush_messages(dude);
    }
    debug_message(strip_colours(str));
}

varargs int tn(string str, string col, object room, int mclass){
    string prefix;
    if(!col) col = "magenta";
    prefix = tc_color_logic(col);
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

varargs int debug(mixed msg, string color){
    object *players = filter(users(), (: $1->GetProperty("debug") :) );
    string prevob = "";
    if(!sizeof(players)) return 0;
    prevob = file_name(previous_object());
    if(!color || !sizeof(color)) color = "b_blue%^green";
    foreach(object guy in players){
        tc("%^B_BLACK%^BOLD%^WHITE%^DEBUG: %^RESET%^ " + prevob
                + "\n" + tc_color_logic(color) + msg, color, guy);
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

varargs void tell_room(mixed ob, mixed str, mixed exclude){
    if(!ob ) return;
    if(stringp(ob) && catch(ob = load_object(ob))) return;
    if(ob) ob->eventPrint(str, MSG_ENV, exclude);
}

varargs void shout(mixed str, mixed exclude) {
    if(objectp(exclude)) exclude = ({ exclude });
    else if(!pointerp(exclude)) exclude = ({});
    if(this_player()) exclude += ({ this_player() });
    users()->eventPrint(str + "", MSG_CONV, exclude);
}

void write2(mixed str){
    object player = this_player();
    if(!player) return;
    player->eventReceive(str);
}
