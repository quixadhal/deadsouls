/*    /lib/fishing.c
 *    from the Dead Souls LPC Library
 *    a room which allows fishing in it
 *    created by Descartes of Borg 950529
 *    Version: @(#) fishing.c 1.7@(#)
 *    Last modified: 96/10/30
 */

#include <lib.h>
#include "include/fishing.h"

inherit LIB_ROOM;

static private int MaxFishing, Speed, Chance, fishing_counter;
static private mapping Fishing, Fish;

static void create(){
    room::create();
    MaxFishing = 10;
    Speed = 10;
    Chance = 0;
    fishing_counter = 0;
    Fish = ([]);
    Fishing = ([]);
}

void heart_beat(){
    mapping tmp;
    object pole;
    string fisher;

    room::CheckActions();

    fishing_counter++;
    if(fishing_counter < Speed) return;
    fishing_counter = 0;

    if( !sizeof(Fishing) ){
        return;
    }
    tmp = Fishing;
    Fishing = ([]);
    foreach(fisher, pole in tmp){
        object ob;

        if( !fisher ) continue;
        if( !(ob = present(fisher)) || !living(ob) || !pole ) continue;
        if( !present(pole, ob) ) continue;
        if( pole->GetBroken() ) continue;
        if( (object)ob->GetInCombat() ){
            message("my_action", "You are no longer fishing.", ob);
            RemoveFishing(ob);
            continue;
        }
        if( (object)ob->GetSleeping() ){
            message("my_action", "You are no longer fishing.", ob);
            RemoveFishing(ob);
            continue;
        }
        Fishing[fisher] = pole;
    }
    if( !sizeof(Fishing) ){
        return;
    }
    foreach(fisher, pole in Fishing){
        object who;
        string fish;
        int chance, x, y, i;
        int pro;

        who = present(fisher, this_object());
        /* if this room is impossible to fish, or if using a non-fishing 
         * device, no fishing can really occur
         */
        if( !Chance || !(x = pole->eventFish(who)) ) chance = 0;
        else {
            pro = (Chance + x + who->GetStatLevel("luck"))/3;
            chance = (Chance + x + who->GetStatLevel("luck")) /
                (1 + random(5));
        }
        /* Give extra weight to fishing skill */
        if( chance ) 
            chance = random(chance/2 + who->GetSkillLevel("fishing")/2);
        y = 0;
        foreach(fish, x in Fish) y += x;
        y = random(y);
        if( y < 1 || chance <= random(100) ){
            who->eventTrainSkill("fishing", pro, 100-Chance/(y+1), 0);
            send_messages("cast", "$agent_name $agent_verb again, hoping "
                    "for better luck.", who, 0, this_object());
            continue;
        }
        i = 0;
        foreach(fish, x in Fish){
            i += x;
            if( x >= y ) break;
        }
        who->eventTrainSkill("fishing", pro, (100-Chance)/(i+1), 1);
        send_messages("struggle", "$agent_name $agent_verb with "
                "something on $agent_possessive " +
                pole->GetKeyName() + ".", who, 0, this_object());
        Fish[fish]--;
        call_out( (: eventCatch, who, fish, pole :), 1 );
    }
}

    mixed CanCast(object who, string where){
        if( this_player()->GetInCombat() ) 
            return "You are too busy to fish!";
        if( Fishing[this_player()->GetKeyName()] )
            return "You are already fishing!";
        if( GetMaxFishing() <= sizeof(Fishing) ) 
            return "It is too crowded here to fish.";
        return 1;
    }

mixed CanStop(object who, string str){
    if( str != "fishing" ) return 0;
    str = this_player()->GetKeyName();
    if( !Fishing[str] ) return "You are not fishing!";
    return 1;
}

int CanRelease(object who){
    if(room::CanRelease()) RemoveFishing(who);
    return 1;
}

mixed eventCast(object who, object pole, string str){
    send_messages(({ "cast", "start" }),
            "$agent_name $agent_verb $agent_possessive " +
            pole->GetKeyName() + " and $agent_verb fishing.", who, 0,
            this_object());
    SetFishing(who, pole);
    return 1;
}

static void eventCatch(object who, string fish, object pole){
    object food;

    if( !who || !present(who) ) return;
    if( !pole || !present(pole, who) ){
        message("my_action", "Having given up " + pole->GetShort() + 
                ", you lose your catch!", who);
        return;
    }
    if( !(pole->eventCatch(who, fish)) ) return;
    food=new(fish);
    RemoveFishing(who);
    who->AddSkillPoints("fishing", fish->GetFight()+fish->GetMass());
    message("my_action", "You find " + fish->GetShort() + " on " +
            pole->GetShort() + "!", who);
    message("other_action", who->GetName() + " finds " + 
            fish->GetShort() + " on " + pole->GetShort() + 
            "!", this_object(), ({ who }));
    if( !(food->eventMove(who)) ){
        message("my_action", "You drop " + food->GetShort() + "!",
                who);
        message("other_action", who->GetName() + " drops " +
                food->GetShort() + "!", this_object(), ({ who }) );
        food->eventMove(this_object());
    }
}

mixed eventStop(object who, string str){
    RemoveFishing(this_player());
    message("my_action", "You stop fishing.", who);
    message("other_action", who->GetName() + " stops "
            "fishing.", this_object(), ({ who }) );
    return 1;
}

int SetChance(int x){ return (Chance = x); }

int GetChance(){ return Chance; }

mapping SetFish(mapping mp){ return (Fish = mp); }

int AddFish(string fish, int x){
    if( !Fish[fish] ) Fish[fish] = x;
    else Fish[fish] += x;
    return Fish[fish];
}

mapping RemoveFish(string fish){
    if( Fish[fish] ) map_delete(Fish, fish);
    return Fish;
}

mapping GetFish(){ return Fish; }

mapping SetFishing(object who, object pole){
    if( !living(who) ) return Fishing;
    if( !query_heart_beat() ) set_heart_beat(1);
    Fishing[who->GetKeyName()] = pole;
    return Fishing;
}

mapping RemoveFishing(object who){
    string str;

    if( !who || !objectp(who) || !living(who) ) return Fishing;
    if( Fishing[(str = who->GetKeyName())] ) 
        map_delete(Fishing, str);
    if( !sizeof(Fishing) ) return ([]);
    return Fishing;
}

int SetMaxFishing(int x){ return (MaxFishing = x); }

int GetMaxFishing(){ return MaxFishing; }

int SetSpeed(int x){ return (Speed = x); }

int GetSpeed(){ return Speed; }
