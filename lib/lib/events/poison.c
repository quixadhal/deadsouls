/*    /lib/events/poison.c
 *    From the Dead Souls Mud Library
 *    Objects which can respond to poisoning
 *    Created by Descartes of Borg 970101
 *    Version: @(#) poison.c 1.1@(#)
 *    Last modified: 97/01/01
 */

private int Poison = 0;

int AddPoison(int x){
    Poison += x;
    if( Poison < 1 ){
        Poison = 0;
    }
    return Poison;
}

int GetPoison(){
    return Poison;
}

int SetPoison(int x){
    return (Poison = x);
}

string array GetSave(){
    return ({ "Poison" });
}

mixed CanPoison(object who){
    return 1;
}

mixed eventPoison(object who, object agent, int strength){
    send_messages("spread", "$agent_name $agent_verb some poison onto "
            "$target_name.", who, this_object(), environment(who));
    AddPoison(strength);
    return 1;
}

mixed direct_poison_obj_with_obj(){
    return CanPoison(this_player());
}
