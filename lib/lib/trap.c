/*    /lib/trap.c
 *    From the Dead Souls Object Library
 *    Allows living things to be trapped and stored across reboots
 *    Created by Descartes of Borg 961010
 *    Version: @(#) trap.c 1.5@(#)
 *    Last modified: 96/12/03
 */

#include <lib.h>
#include "include/trap.h"

inherit LIB_STORAGE;

int EscapeChance = 0;
int MaxCapture = 0;

static void create(){
    storage::create();
    if( sizeof(GetCaptives()) ){
        set_heart_beat(2);
    }
}

static void heart_beat(){
    if( !sizeof(GetCaptives()) ){
        set_heart_beat(0);
        return;
    }
    if( !GetClosed() ){
        if( GetEscapeChance() > (1 + random(100)) ){
            eventEscape();
        }
    }
}

mixed indirect_capture_liv_word_obj(object target){
    if( environment() != this_player() ){
        return "#You do not have " + GetShort() + ".";
    }
    if( !target ){
        return 1;
    }
    return CanCapture(this_player(), target);
}

mixed indirect_free_liv_from_obj(object target){
    if( environment() != this_player() ){
        return "#You do not have " + GetShort() + ".";
    }
    if( !target ){
        return 1;
    }
    return CanFree(this_player(), target);
}

mixed CanCapture(object who, object target){
    if( environment(who) != environment(target) ){
        return "#You cannot get to it.";
    }
    if( GetClosed() ){
        return capitalize(GetShort()) + " is closed.";
    }
    if( sizeof(GetCaptives()) >= MaxCapture ){
        return "You cannot capture anything else with " + GetShort() + ".";
    }
    return 1;
}

mixed CanFree(object who, object target){
    if( environment(target) != this_object() ){
        return "#" + target->GetName() + " is not in " + GetShort() + ".";
    }
    if( GetClosed() ){
        return "You must open " + GetShort() + " before you can free "
            "things from it.";
    }
    return 1;
}

mixed eventCapture(object who, object target){

    if( !target->eventMove(this_object()) ){
        return target->GetShort() + " will not fit!";
    }
    who->eventPrint("You capture " + target->GetName() + " in " +
            GetShort() + "!");
    environment(who)->eventPrint(who->GetName() + " captures " +
            target->GetName() + " in " + GetShort() +
            ".", ({ who, target }));
    target->eventPrint(who->GetName() + " captures you in " + GetShort() +".");
    set_heart_beat(2);
    return 1;
}

mixed eventEscape(){
    object array captives;
    object captive, env;

    if( GetClosed() ){
        return 0;
    }
    captives = GetCaptives();
    if( !sizeof(captives) ){
        return 0;
    }
    captive = captives[random(sizeof(captives))];
    if( !captive ){
        return 0;
    }
    env = environment();
    if( living(env) ){
        env = environment(env);
        if( !env ){
            return 0;
        }
    }
    if( captive->eventMove(env) != 1 ){
        return 0;
    }
    env->eventPrint(captive->GetShort() + " escapes from " + GetShort() + ".");
    return 1;
}

mixed eventFree(object who, string target){
    target->eventMove(environment(who));
    who->eventPrint("You release " + target->GetName() + " from " +
            GetShort() + ".");
    environment(who)->eventPrint(who->GetName() + " releases " +
            target->GetName() + " from " + GetShort()+".",
            ({ who, target }));
    target->eventPrint(who->GetName() + " releases you from " + GetShort() +".");
    return 1;
}

object array GetCaptives(){
    return filter(all_inventory(), (: living :));
}

int GetEscapeChance(){
    return EscapeChance;
}

int SetEscapeChance(int x){
    return (EscapeChance = x);
}

int GetMaxCapture(){
    return MaxCapture;
}

int SetMaxCapture(int x){
    return (MaxCapture = x);
}
