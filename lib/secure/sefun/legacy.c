#include <rooms.h>
#include <daemons.h>

void set(mixed arg1, mixed arg2){
    arg1 = capitalize(arg1);
    call_other( previous_object(), ({ "Set"+arg1, arg2 }) );
    return;
}

void query(mixed arg1, mixed arg2){
    arg1 = capitalize(arg1);
    call_other( previous_object(), ({ "Get"+arg1, arg2 }) );
    return;
}

void personal_log(string str){
    tell_room(ROOM_ARCH,str);
    return;
}

void add_sky_event(function f){
    SEASONS_D->AddTimeEvent("night", f);
}


