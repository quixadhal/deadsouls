#include <cfg.h>
#include <daemons.h>
#include <localtime.h>

string *zones_cfg = explode((read_file(CFG_TIMEZONES)|| ""), "\n");

varargs string timestamp(mixed tz){
    string rawtz;
    int *t, offset;

    if(tz){
        tz = upper_case(tz);
        if(member_array(tz, zones_cfg) == -1) tz = 0;
    }

    if(!tz) rawtz = query_tz();
    else rawtz = tz;
    if(!rawtz) rawtz = upper_case(localtime(time())[LT_ZONE]);
    offset = TIME_D->GetOffset(rawtz);
    offset += EXTRA_TIME_OFFSET;
    t = localtime(time()+(offset*3600));
    return sprintf("%04d.%02d.%02d-%02d.%02d,%02d", t[LT_YEAR], (t[LT_MON])+1, t[LT_MDAY], t[LT_HOUR], t[LT_MIN], t[LT_SEC]);
}
