#include <lib.h>

inherit LIB_DAEMON;
string savefile = "/save/meetings.o";
mapping schedule = ([]);

static void create() {
    daemon::create();
    restore_object(savefile,1);
}

mapping GetScheduleMapping(){
    return copy(schedule);
}

//mapping AddMeeting(string str){
//int tmpint;
//string *zonearray = explode(read_file("/cfg/timezones.cfg"),"\n");
//string date_str, time_str, tz_str;
//if(!str) return GetScheduleMapping();
//if(sscanf(str, "%s %s %s",date_str, time_str, tz_str) != 3) return GetScheduleMapping();
//date_str = itoa(atoi(date_str));
//if(sizeof(date_str) != 8) return GetScheduleMapping();
//if(member_array(tz_str,zonearray) == -1) return GetScheduleMapping();
//if(sscanf(time_str,"



