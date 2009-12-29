/* This file cleane dup by Jonez,
 * 21Mar06
 */

#include <lib.h>
#include <privs.h>
#include <cfg.h>
#include <daemons.h>

string tz;
string *zonearray = explode((read_file(CFG_TIMEZONES)|| ""), "\n");
mapping months = ([ 
        0 : "January",
        1 : "February",
        2 : "March",
        3 : "April",
        4 : "May",
        5 : "June",
        6 : "July",
        7 : "August",
        8 : "September",
        9 : "October",
        10 : "November",
        11 : "December",]);
mapping days = ([
        0 : "Sunday",
        1 : "Monday",
        2 : "Tuesday",
        3 : "Wednesday",
        4 : "Thursday",
        5 : "Friday",
        6 : "Saturday",
        ]);

string query_tz(){
    if(tz) return tz;
    if (file_size("/cfg/timezone.cfg") > 0) 
        tz = read_file("/cfg/timezone.cfg")[0..2];
    if (!tz) tz = "GMT";
    return tz;
}

varargs mixed local_ctime(int i, string tzone){
    int x, offset;
    if(!tzone || !valid_timezone(tzone)) tzone = query_tz();
    offset = TIME_D->GetOffset(tzone);
    offset += EXTRA_TIME_OFFSET;
    if(query_os_type() != "windows" ) x = offset * 3600;
    else x = 0;
    return ctime(i + x);
}

varargs mixed local_time(mixed val){
    mixed stuff = ({});
    string tzone;
    int offset;
    if(stringp(val)) {
        val = upper_case(val);
        if(member_array(val, zonearray) != -1) tzone = val;
    }
    if(!tzone) tzone = query_tz();
    offset = TIME_D->GetOffset(tzone);
    offset += EXTRA_TIME_OFFSET;
    offset *= 3600;

    stuff = localtime(time() + offset);
    stuff[9] = tzone;
    return stuff;
}

int valid_timezone(string str){
    if(!str || str == "") return 0;
    str = upper_case(str)[0..2];
    if(member_array(str,zonearray) == -1) {
        return 0;
    }
    else return 1;
}

string set_tz(string str){
    if(!str) str = "";
    if( str != "" && !valid_timezone(str)) {
        return "Invalid time zone.";
    }
    tz = str;
    if( !(master()->valid_apply(({ "PRIV_ASSIST", "PRIV_SECURE" }))) ) 
        error("Illegal attempt to modify timezone: "+get_stack()+" "+identify(previous_object(-1)));
    unguarded( (: write_file("/cfg/timezone.cfg",tz,1) :) );
    return "Mud time zone is now "+read_file("/cfg/timezone.cfg");
}

varargs string system_month(int i, int abbr){
    if(!months[i]) return "";
    if(!abbr) return months[i];
    else return months[i][0..2];
}

varargs string system_day(int i, int abbr){
    if(!days[i]) return "";
    if(!abbr) return days[i];
    else return days[i][0..2];
}
