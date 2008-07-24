#include <config.h>
#include <daemons.h>
#include <localtime.h>

/**
 * 2006-03-22, jonez
 *  - changed prototype so it does not accept an argument since that was not
 *    used anywhere in the entire lib.
 */

string timestamp(){
    string rawtz;
    int *t, gmtoff, offset;

    gmtoff = localtime(time())[LT_GMTOFF];

    // if the gmtoffset is set to zero by the driver, then we need to
    // calculate an offset
    if (gmtoff == 0){
        // if the timezone.cfg file exists, use it else default to GMT
        rawtz = query_tz();
        offset = TIME_D->GetOffset(rawtz);
        offset += EXTRA_TIME_OFFSET;
    }
    else {
        rawtz = upper_case(localtime(time())[LT_ZONE]);
        offset = 0;
    }

    t = localtime(time()+(offset*3600));
    return sprintf("%04d.%02d.%02d-%02d.%02d,%02d", t[LT_YEAR], (t[LT_MON])+1, t[LT_MDAY], t[LT_HOUR], t[LT_MIN], t[LT_SEC]);
}
