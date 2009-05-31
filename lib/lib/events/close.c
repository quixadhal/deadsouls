/*    /lib/events/close.c
 *    from the Dead Souls Mud Library
 *    handles closing and opening events
 *    created by Descartes of Borg 960115
 *    Version: @(#) close.c 1.5@(#)
 *    Last modified: 96/12/23
 */

int Closed;

// abstract methods
string GetDefiniteShort();
// end abstract methods

int GetClosed(){
    return Closed;
}

int GetOpen(){
    return !(Closed);
    if(GetClosed()) return 0;
    else return 1;
}

int SetClosed(int x){ 
    Closed = x;
    parse_refresh();
    return Closed;
}

varargs mixed CanClose(object who, string id){

    if(who && environment() && environment() != environment(who) &&
            environment() != who)
        return "#That's not accessible to you.";

    if( Closed ){
        id = capitalize(GetDefiniteShort()) + " is already closed.";
        return id;
    }
    else return 1;
}

varargs mixed CanOpen(object who, object tool){
    if(environment() && environment() != environment(who) &&
            environment() != who)
        return "#That's not accessible to you.";

    if( !Closed ){
        return capitalize(GetDefiniteShort()) + " is already open.";
    }
    else return 1;
}

mixed eventClose(object who){
    if( !SetClosed(1) ){
        return 0;
    }
    send_messages("close", "$agent_name $agent_verb $target_name.",
            who, this_object(), environment(who));
    return 1;
}

varargs mixed eventOpen(object who, object tool){
    if( SetClosed(0) ){
        return 0;
    }
    send_messages("open", "$agent_name $agent_verb $target_name.",
            who, this_object(), environment(who));
    return 1;
}

int inventory_accessible(){
    return !GetClosed();
}

int inventory_visible(){
    return !GetClosed();
}

int SetOpen(int x){
    if(x) Closed = 0;
    else Closed = 1;
    parse_refresh();
    return Closed;
}


mixed direct_close_obj(object target){
    return CanClose(this_player());
}

mixed direct_open_obj(object target){
    return CanOpen(this_player());
}

mixed direct_open_obj_with_obj(object target, object tool){
    return CanOpen(this_player(), tool);
}
