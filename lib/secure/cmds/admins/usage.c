
#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    mapping info;
    string *cles;
    string ret;
    float x, fraction;

    if(!archp(previous_object())) return 0;
    info = rusage();
    cles = keys(info);

    foreach(string schlussel in cles){
        write(schlussel + ": "+info[schlussel]);
    }

    x = (info["utime"] + info["stime"])/1000;
    fraction = x / uptime();
    ret = sprintf("\nsingle CPU usage: %2.2f%%\n", fraction * 100);
    return ret;

}


