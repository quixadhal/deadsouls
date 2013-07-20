/*    /lib/events/lock.c
 *    from the Dead Souls Mud Library
 *    Handles lock/unlock/pick events
 *    Created by Descartes of Borg 961221
 *    Version: @(#) lock.c 1.4@(#)
 *    Last modified: 96/12/23
 */
#include <damage_types.h>

private string array    Keys         = ({});
private int             Locked       = 0;
private int             LockStrength = 0;
private static function Pick         = 0;

// abstract methods
string GetDefiniteShort();
// end abstract methods

string array AddKey(string key){
    return (Keys = ({ Keys..., key }));
}

varargs string array GetKeys(string unused){
    return Keys;
}

varargs string array SetKeys(mixed array args...){
    if( !args ){
        error("Bad argument 1 to SetKeys().\n");
    }
    Keys = ({});
    foreach(mixed val in args){
        if( !val ){
            continue;
        }
        if( arrayp(val) ){
            Keys = Keys + val;
        }
        else {
            Keys = ({ Keys..., val });
        }
    }
    return Keys;
}

int GetLocked(){
    return Locked;
}

int SetLocked(int x){
    return (Locked = x);
}

int GetLockStrength(){
    return LockStrength;
}

int SetLockStrength(int x){
    return (LockStrength = x);
}

function GetPick(){
    return Pick;
}

function SetPick(function f){
    return (Pick = f);
}

string array GetSave(){
    return ({ "Locked", "Keys", "LockStrength" });
}

mixed CanLock(object who, string id){
    if( GetLocked() ){
        return "It is already locked.";
    }
    return 1;
}

mixed CanPick(object who, string id){
    if( !GetLocked() ){
        return "Pick it when it is not even locked?";
    }
    return 1;
}

varargs mixed CanUnlock(object who, string id, object key){
    if( !GetLocked() ){
        return "It is already unlocked.";
    }
    return 1;
}

varargs mixed eventLock(object who, mixed arg1, mixed arg2){
    string array ids = ({});
    object key;

    if(objectp(arg1)) key = arg1;
    else if(objectp(arg2)) key = arg2;
    else return "There seems to be a problem with unlocking things.";

    ids = key->GetId();
    ids += ({ key->GetName() });

    if(key->LockFun(1,key,lower_case(this_object()->GetName()))){
        return 1;
    }

    if( !sizeof(ids & GetKeys()) ){
        send_messages("try", "$agent_name $agent_verb to lock $target_name "
                "with " + key->GetShort() + ", but it does not work.",
                who, this_object(), environment(who));
    }
    else {
        mixed tmp;

        send_messages("attempt", "$agent_name $agent_verb to lock "
                "$target_name with " + key->GetShort() + ".",
                who, this_object(), environment(who));
        tmp = key->eventLockLock(who, this_object());
        if( tmp != 1 ){
            if( tmp ){
                who->eventPrint(tmp);
            }
            return 1;
        }
        if( !SetLocked(1) ){
            return 0;
        }
        environment(who)->eventPrint(capitalize(GetDefiniteShort()) +
                " locks.");
    }
    return 1;
}

varargs mixed eventPick(object who, string id, object tool){
    mixed tmp;
    int strength;
    mixed *prehensiles = ({});
    string limb;
    string short;
    object prev = previous_object();
    if(prev->isDummy()) short = (prev->GetShort() || prev->GetName());

    if(!who) return 0;

    prehensiles = who->GetWieldingLimbs();
    if(!sizeof(prehensiles)){
        who->eventPrint("You lack prehensile limbs with which to do that.");
        return 1;
    }

    limb = prehensiles[random(sizeof(prehensiles))];

    if( !tool ){
        strength = who->GetSkillLevel("stealth");
    }
    else {
        tmp = tool->eventPickLock(who, id, this_object());
        strength = tmp + who->GetSkillLevel("stealth");
    }
    who->AddSkillPoints("stealth", strength + 1);
    who->AddStaminaPoints(-LockStrength/10.0);
    if( Pick ){
        tmp = evaluate(Pick, who, id, tool, strength);
        if( tmp != 1 ){
            if( !tmp ){
                who->eventPrint("You fail to pick it.");
                return 1;
            }
            return tmp;
        }
        who->AddSkillPoints("stealth", 2*(LockStrength + strength));
        if( SetLocked(0) ){
            return 0;
        }
        return 1;
    }
    if( strength > ( LockStrength / 10 + random(LockStrength) ) ){
        who->AddSkillPoints("stealth", 2*(LockStrength + strength));	
        SetLocked(0);
        send_messages("pick", "$agent_name $agent_verb the lock on "+
                (short ? short : "$target_name") +"!",
                who, this_object(), environment(who));
        return 1;
    }
    send_messages("fail", "$agent_name $agent_verb in $agent_possessive "
            "attempt to pick the lock on "+
            (short ? short : "$target_name") +".",
            who, this_object(), environment(who));
    if( random(100) > strength ){
        send_messages("cut", "$agent_name $agent_verb $agent_reflexive "
                "on the lock.", who, this_object(), environment(who));
        who->eventReceiveDamage(this_object(), PIERCE, random(10) + 1, 0, limb);
    }
    return 1;
}

varargs mixed eventUnlock(object who, mixed arg1, mixed arg2){
    string array ids = ({});
    object key;

    if(objectp(arg1)) key = arg1;
    else if(objectp(arg2)) key = arg2;
    else return "There seems to be a problem with unlocking things.";

    ids = key->GetId();
    ids += ({ key->GetName() });

    if(key->UnLockFun(1,key,lower_case(this_object()->GetName()))){
        return 1;
    }

    if( !sizeof(ids & GetKeys()) ){
        send_messages("attempt", "$agent_name $agent_verb to unlock "
                "$target_name with " + key->GetShort() + ", but it "
                "does not work.", who, this_object(), environment(who));
    }
    else {
        mixed tmp;

        send_messages("attempt", "$agent_name $agent_verb $target_name with "+
                key->GetShort() + ".", who, this_object(),
                environment(who));
        tmp = key->eventUnlockLock(who, this_object());
        if( tmp != 1 ){
            if( tmp ){
                who->eventPrint(tmp);
            }
            return 1;
        }
        if( SetLocked(0) ){
            return 0;
        }
        environment(who)->eventPrint(capitalize(GetDefiniteShort()) +
                " comes unlocked.");
    }
    return 1;
}

varargs mixed direct_lock_obj_with_obj(object target, object key, mixed id...){
    return CanLock(this_player(), remove_article(lower_case(id[0])));
}

varargs mixed direct_lock_obj_with_str(object target, mixed key, mixed id...){
    return CanLock(this_player(), remove_article(lower_case(id[0])));
}

varargs mixed direct_pick_str_on_obj(string str, object target, string str2,
        string id){
    if( remove_article(lower_case(str)) != "lock" ){
        return "Pick the what?";
    }
    return CanPick(this_player(), id);
}

varargs mixed direct_pick_str_on_obj_with_obj(string str, object target, object tool,
        string str2, string targ_id){
    if( remove_article(lower_case(str)) != "lock" ){
        return "Pick the what?";
    }
    targ_id = remove_article(lower_case(targ_id));
    return CanPick(this_player(), targ_id);
}

varargs mixed direct_unlock_obj_with_obj(object target, object key, string id, string key2){
    return CanUnlock(this_player(), remove_article(lower_case(id)));
}

varargs mixed direct_unlock_obj_with_str(object target, object key, string id, string key2){
    return CanUnlock(this_player(), remove_article(lower_case(id)));
}

mixed direct_wizlock_obj(){ return 1; }
mixed direct_wizunlock_obj(){ return 1; }
