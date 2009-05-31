#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <localtime.h>
#include <daemons.h>

inherit LIB_ARMOR;

int eventReadWatch();

static void create(){
    armor::create();
    SetKeyName("pocket watch");
    SetId(({"watch","pocketwatch","pocket watch","timepiece"}));
    SetShort("a pocket watch");
    SetLong("This is a beautifully decorated and intricately carved "+
            "pocket watch. It glows with a bright, polished bronze "+
            "luster. It is quietly ticking the seconds, and it looks like you can get the "+
            "accurate time by reading the watch.");
    SetMass(10);
    SetBaseCost("silver",40);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_AMULET);
    SetDefaultRead( (: eventReadWatch :) );
}
void init(){
    ::init();
}

int eventReadWatch(){
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

    write("According to the watch, it is "+hour_string+" "+minute_string+".");

    return 1;
}
