/*    /adm/simul_efun/log_file.c
 *    from Dead Souls Mudlib 3.2
 *    a simul_efun for logging to the log dir
 *    created by Descartes of Borg 940121
 */

#include <dirs.h>
#include <privs.h>

void log_file(string fl, string msg) {
    if(!stringp(fl)) error("Bad argument 1 to log_file().\n");
    if(!stringp(msg)) msg = identify(msg);
    if(strsrch(fl, "..") != -1) error("Illegal file reference.\n");
    if(userp(previous_object(0)) || previous_object(0) == master())
      fl = DIR_LOGS + "/" + fl;
    else if((int)master()->valid_apply(({ PRIV_CMDS, PRIV_MUDLIB }))) 
      fl = DIR_LOGS+"/"+fl;
    else if(member_array(PRIV_SECURE,explode(query_privs(previous_object()),":"))
      != -1) fl = DIR_LOGS+"/"+fl;
    else fl = DIR_LOGS+"/open/"+fl;
    master()->master_log_file(fl, msg);
}
