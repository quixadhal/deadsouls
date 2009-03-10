/*    /lib/events/dig.c
 *    From the Dead Souls Mud Library
 *    Handles things that can be dug
 *    Created by Descartes of Borg 961231
 *    Version: @(#) dig.c 1.1@(#)
 *    Last modified: 97/01/01
 */

private static mixed Dig = 0;

// abstract methods
string GetShort();
// end abstract methods

mixed GetDig(){
    return Dig;
}

int RemoveDig(){
    Dig = 0;
    return 1;
}

mixed SetDig(mixed val){
    if( !stringp(val) && !arrayp(val) && !functionp(val) ){
        error("Bad argument 1 to SetDig().\n\tExpected: string or function "
                "or mixed array, Got: " + typeof(val) + "\n");
    }
    return (Dig = val);
}

mixed CanDig(object who){
    if( !Dig ){
        return 0;
    }
    else {
        return 1;
    }
}

mixed eventDig(object who, object tool){
    mixed val = Dig;

    if( arrayp(val) ){
        val = val[query_night()];
    }
    if( stringp(val) ){
        environment(who)->eventPrint(who->GetName() + " digs " +
                GetShort() + " with " +
                tool->GetShort() + ".", who);
        who->eventPrint(val);
    }
    else {
        return evaluate(val, who, tool);
    }
}

mixed direct_dig_obj_with_obj(){
    return CanDig(this_player());
}
