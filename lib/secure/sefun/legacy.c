#include ROOMS_H
#include <daemons.h>

void set(mixed arg1, mixed arg2){
    string s1, s2;
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
    return args;
}

mixed set_verbs(string *args){
    previous_object()->SetCombatAction(5, args);
}

string day(){
    return SEASONS_D->GetDayName();
}

int date(){
    return SEASONS_D->GetDay();
}

string month(){
    return SEASONS_D->GetMonth();
}

int year(){
    return SEASONS_D->GetCurrentYear();
}

varargs int minutes(int x){
    if(!x) x = time();
    return SEASONS_D->GetMinutes(x);
}

varargs string season(int x){
    if(!x) x = time();
    return SEASONS_D->GetSeason(x);
}

varargs int hour(int x){
    if(!x) x = time();
    return SEASONS_D->GetHour(x);
}

string query_time(){
    int hour, minutes;
    int *time_of_day;
    string hour_string, minute_string;

    time_of_day = SEASONS_D->GetMudTime();
    hour = time_of_day[0];
    minutes = time_of_day[1];

    if(hour > 12) hour -= 12;

    hour_string = cardinal(hour);
    switch(minutes){
        case 0 : minute_string ="o'clock"; break;
        case 1 : minute_string = hour_string;hour_string ="one minute past"; break;
        case 2 : minute_string = hour_string;hour_string ="two minutes past"; break;
        case 3 : minute_string = hour_string;hour_string ="three minutes past"; break;
        case 4 : minute_string = hour_string;hour_string ="four minutes past"; break;
        case 5 : minute_string = hour_string;hour_string ="five past"; break;
        case 6 : minute_string = hour_string;hour_string ="six minutes past"; break;
        case 7 : minute_string = hour_string;hour_string ="seven minutes past"; break;
        case 8 : minute_string = hour_string;hour_string ="eight minutes past"; break;
        case 9 : minute_string = hour_string;hour_string ="nine minutes past"; break;
        case 10 : minute_string = hour_string;hour_string ="ten past"; break;
        case 15 : minute_string = hour_string;hour_string ="quarter past"; break;
        case 45 : minute_string = cardinal(hour+1);hour_string ="quarter of"; break;
        case 50 : minute_string = cardinal(hour+1);;hour_string ="ten of"; break;
        case 55 : minute_string = cardinal(hour+1);hour_string ="five of"; break;
        default : minute_string = cardinal(minutes);break;
    }
    if(minute_string == "thirteen") minute_string = "one";
    return hour_string+" "+minute_string;
}
