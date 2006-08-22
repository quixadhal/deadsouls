#include <rooms.h>
#include <daemons.h>

void set(mixed arg1, mixed arg2){
    string s1, s2;
    tc("arg1: "+identify(arg1));
    tc("arg2: "+identify(arg2));
    arg1 = capitalize(arg1);
    if(!grepp(arg1,"/")) call_other( previous_object(), ({ "Set"+arg1, arg2 }) );
    else {
	sscanf(arg1,"%s/%s",s1,s2);
	arg1 = s1;
	arg2 = "([\""+s2+"\":"+identify(arg2)+"])";
	call_other( previous_object(), ({ "Set"+arg1, arg2 }) );
    }
    return;
}

void add(mixed arg1, mixed arg2){
    string s1, s2;
    tc("arg1: "+identify(arg1));
    tc("arg2: "+identify(arg2));
    arg1 = capitalize(arg1);
    if(!grepp(arg1,"/")) call_other( previous_object(), ({ "Add"+arg1, arg2 }) );
    else {
	sscanf(arg1,"%s/%s",s1,s2);
	arg1 = s1;
	arg2 = "([\""+s2+"\":"+identify(arg2)+"])";
	call_other( previous_object(), ({ "Add"+arg1, arg2 }) );
    }
    return;
}

void query(mixed arg1, mixed arg2){
    arg1 = capitalize(arg1);
    call_other( previous_object(), ({ "Get"+arg1, arg2 }) );
    return;
}

varargs mixed dump_variable(mixed foo, mixed bar){
    return identify(foo);
}

void personal_log(string str){
    tell_room(ROOM_ARCH,str);
    return;
}

void add_sky_event(function f){
    SEASONS_D->AddTimeEvent("night", f);
}

mixed seteuid(mixed args){
    return args;
}

mixed getuid(mixed args){
    return args;
}

mixed geteuid(mixed args){
    //if(objectp(args)) return args->GetKeyName();
    return args;
}

mixed set_verbs(string *args){
    previous_object()->SetCombatAction(5, args);
}
