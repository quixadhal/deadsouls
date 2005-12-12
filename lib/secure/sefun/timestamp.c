string timestamp(string str){
    string *rawstr;
    string stamp,rawtz,tzone,l_time;
    int timediff,offset,foo,bar;
    if( file_size("/cfg/timezone.cfg") > 0 ) rawtz = read_file("/cfg/timezone.cfg");
    if(str && str != "") rawtz = upper_case(str);
    if(sizeof(rawtz) < 3) return "Please use a valid timezone, e.g. 'PDT'";
    tzone = rawtz[0..2];
    if(!tzone) return "Problem resolving timezone";
    offset = "/daemon/time"-> GetOffset(tzone);
    if(!offset && tzone !="GMT") return "Problem calculating timezone offset";
    // the following appears to be necessary for linux
    //offset +=10;
    timediff = offset * 3600;
    l_time=ctime(time() + timediff);
    if( sscanf(l_time,"%s  %s",foo,bar) ) l_time = foo+" 0"+bar;
    rawstr=explode(l_time," ");
    stamp=rawstr[2]+rawstr[1]+rawstr[4]+"-"+rawstr[3];
    return stamp;
}
