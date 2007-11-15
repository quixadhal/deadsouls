/*    /secure/sefun/value.c
 *    from the Dead Souls LPC Library
 *    data conversion simuls
 *    created by Descartes of Borg 950509
 */

#include "sefun.h"

mixed convert_value(string val) {
    return 1;
}

int round(string f){
    int frac,num;
    string str;
    if(strsrch(f,".") == -1) {
        if(!sscanf(f,"%d",num)) return 0;
        else return num;
    }

    if(sscanf(f,"%d.%s",num,str) < 2) sscanf(f,".%s",str);

    if(!str || str == "") return -1;

    if(!sscanf(str[0],"%d",frac)) return -2;

    if(frac > 4) num++;
    else num--;
    return num;
}

