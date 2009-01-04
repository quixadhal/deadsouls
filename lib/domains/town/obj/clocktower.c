#include <lib.h>
#include <daemons.h>

inherit LIB_ITEM;

int eventReadClock();

static void create(){
    item::create();
    SetKeyName("clock tower");
    SetId( ({"great clock tower rising majestically into the sky", "great clock of the town", "clock", "tower", "structure", "architecture", "clocktower"}) );
    SetAdjectives( ({"large", "great", "majestic", "impressive", "proud"}) );
    SetShort("a great clock tower rising majestically into the sky");
    SetLong("This is a large clock tower, rising magestically into "+
            "the sky. Some forty feet up you can see the great clock of "+
            "the town, which can be read to know the local time. The "+
            "intricate details of its architecture are deeply impressive, "+
            "and there is little doubt that this great structure is the "+
            "pride of this little town.");
    SetMass(1000000);
    SetBaseCost("silver",40);
    SetDefaultRead( (: eventReadClock :) );
}

void init(){
    ::init();
}

mixed CanGet(object ob) { return "This is a clock tower. It's not gettable.";}

int eventReadClock(){
    int hour, minutes;
    int *time_of_day;
    string hour_string, minute_string;

    time_of_day = SEASONS_D->GetMudTime();
    hour = time_of_day[0];
    minutes = time_of_day[1];

    if(hour > 12) hour -= 12;

    hour_string = cardinal(hour);
    if(hour_string == "zero") hour_string = "twelve";
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

    write("According to the clock, it is "+hour_string+" "+minute_string+".");

    return 1;
}
