string tz;

string query_tz(){
    string zone;
    zone = read_file("/cfg/timezone.cfg")[0..2];
    if(!zone) zone = "GMT";
    return zone;
}

mixed local_time(mixed val){
    string *zonearray;
    string rawtz,tzone,l_time, str;
    int timediff,offset;

    if(stringp(val)) {
	str = val;
	if( file_size("/cfg/timezone.cfg") > 0 ) rawtz = read_file("/cfg/timezone.cfg");
	if(str && str != "") rawtz = upper_case(str);
	if(sizeof(rawtz) < 3) return "Please use a valid timezone, e.g. 'PDT'";
	tzone = rawtz[0..2];
	//write(tzone);
	zonearray = explode(read_file("/cfg/timezones.cfg"),"\n");
	if(member_array(tzone,zonearray) == -1) return "Invalid time zone.";
	if(!tzone || tzone == "") return "Problem resolving null timezone";
	offset = "/daemon/time"-> GetOffset(tzone);
	if(!offset && tzone !="GMT") return "Problem calculating timezone offset";
	// the following appears to be necessary for linux
	//offset +=10;
	timediff = offset * 3600;
	l_time=ctime(time() + timediff);
	return l_time;
    }
    if(intp(val)){
	mixed *stuff;
	offset = "/daemon/time"-> GetOffset(query_tz());
	offset *= 3600;
	stuff = localtime(time()+offset);
	stuff[9] = query_tz();
	return stuff;
    }

}

string set_tz(string str){
    string *zonearray;

    if(!str) return "Time zone unchanged.";

    tz = upper_case(str)[0..2];
    zonearray = explode(read_file("/cfg/timezones.cfg"),"\n");
    if(member_array(tz,zonearray) == -1) {
	return "Invalid time zone. Valid zones are: "+ implode(zonearray," ");
    }

    if(!archp(this_player())) return "You're not permitted to do this.";

    unguarded( (: write_file("/cfg/timezone.cfg",tz,1) :) );
    return "Mud time zone is now "+read_file("/cfg/timezone.cfg");
}
