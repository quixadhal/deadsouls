/* This file cleane dup by Jonez,
 * 21Mar06
*/

#include <lib.h>
#include <privs.h>
#include <config.h>
#include <daemons.h>

string tz;

string query_tz(){
    string zone;
    if (file_size("/cfg/timezone.cfg") > 0) 
	zone = read_file("/cfg/timezone.cfg")[0..2];
    if (!zone) zone = "GMT";
    return zone;
}

mixed local_ctime(int i){
    return ctime(i + ((TIME_D->GetOffset(query_tz()) ) * 3600));
}

mixed local_time(mixed val){
    string *zonearray;
    string tzone,l_time, os;
    int timediff,offset;

    os = query_os_type();

    if(stringp(val)) {
	tzone =  upper_case(val);
	zonearray = explode(read_file("/cfg/timezones.cfg"),"\n");
	if(member_array(tzone,zonearray) == -1) tzone = query_tz();
	if(!tzone || tzone == "") tzone = query_tz();
	offset = TIME_D-> GetOffset(tzone);
	offset += EXTRA_TIME_OFFSET;
	timediff = offset * 3600;
	if(os != "windows") l_time=ctime(time() + timediff);
	else l_time=ctime(time());
	return l_time;
    }
    if(intp(val)){
	mixed *stuff;
	offset = 0;
	if(!LOCAL_TIME){
	    offset = TIME_D-> GetOffset(query_tz());
	    offset *= 3600;
	}
	stuff = localtime(time()+offset);
	stuff[9] = query_tz();
	return stuff;
    }

}

int valid_timezone(string str){
    string *zonearray;
    if(!str || str == "") return 0;
    str = upper_case(str)[0..2];
    zonearray = explode(read_file("/cfg/timezones.cfg"),"\n");
    zonearray += ({""});
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
    if( !((int)master()->valid_apply(({ "PRIV_ASSIST", "PRIV_SECURE" }))) ) 
	error("Illegal attempt to modify timezone: "+get_stack()+" "+identify(previous_object(-1)));
    unguarded( (: write_file("/cfg/timezone.cfg",tz,1) :) );
    return "Mud time zone is now "+read_file("/cfg/timezone.cfg");
}
