/*    /daemon/remotepost.c
 *    from Foundation II
 *    daemon to handle the network to local mail interface
 *    created by Descartes of Borg 940130
 *    modified for IIPS 3.1 940513
 */

#include <lib.h> 
#include <save.h> 
#include <daemons.h> 
#include "remotepost.h"
 
inherit LIB_DAEMON;

private mapping __MailQueue; 
static private mapping __IncomingMail; 

void create() {
    string *muds; 
    int i; 
 
    daemon::create();
    SetNoClean(1);
    __MailQueue = ([]); 
    __IncomingMail = ([]); 
    if(file_exists(sprintf("%s%s", SAVE_MAILQUEUE, __SAVE_EXTENSION__))) 
      restore_mailqueue(); 
    i = sizeof(muds = keys(__MailQueue)); 
    while(i--) __MailQueue[muds[i]][0]["in transit"] = 0; 
  } 
 
string postal_check(string mud) { 
    if(file_name(previous_object()) != SERVICES_D) return 0;
    if(!__MailQueue[mud = replace_string(lower_case(mud), " ", ".")]) return 0;
    if(__MailQueue[mud][0]["in transit"]) return 0; 
    __MailQueue[mud][0]["in transit"] = 1; 
    return __MailQueue[mud][0]["post"][0]; 
  } 
 
string next_post(string mud) { 
    int x; 
    if(file_name(previous_object(0)) != SERVICES_D) return 0;
 
    if(!__MailQueue[mud = replace_string(lower_case(mud), " ", ".")]) return 0;
    x = __MailQueue[mud][0]["in transit"]++; 
    if(sizeof(__MailQueue[mud][0]["post"]) == x) { 
        if( !sizeof(__MailQueue[mud] = __MailQueue[mud][1..]) )
          map_delete(__MailQueue, mud); 
        save_mailqueue(); 
        return 0; 
      } 
    return __MailQueue[mud][0]["post"][x]; 
  } 
 
string resend_post(string mud) { 
    if(file_name(previous_object(0)) != SERVICES_D) return 0;
    if(!__MailQueue[mud = replace_string(lower_case(mud), " ", ".")]) return 0;
    __MailQueue[mud][0]["in transit"] = 1; 
    return __MailQueue[mud][0]["post"][0]; 
  } 
 
void send_post(mapping borg, string mud) {
    string *msg, *tmp; 
    int i, maxi, x, y; 
 
    borg["to"] = convert_names(borg["to"]); 
    borg["cc"] = convert_names(borg["cc"]); 
    borg["from"] = sprintf("%s@%s", convert_name(borg["from"]),
      mud_name());
    tmp = ({ sprintf("||WIZTO:%s||WIZFROM:%s%s||SUBJECT:%s||DATE:%d||MSG:", 
      implode(borg["to"], ","), borg["from"],  
      (sizeof(borg["cc"]) ? implode(borg["cc"], ",") : ""), borg["subject"], 
      time()) }); 
    msg = explode(borg["message"], "\n"); 
    for(i=0, maxi = sizeof(msg); i<maxi; i++) { 
        if((y=strlen(tmp[x])) > 200) { 
            x++; 
            tmp += ({ "||MSG:" }); 
	  } 
        tmp[x] += sprintf("%s\n", msg[i]); 
      } 
    tmp[x] += "||ENDMSG:1"; 
    if(!__MailQueue[mud]) 
      __MailQueue[mud] = ({ ([ "in transit": 0, "post": tmp ]) }); 
    else __MailQueue[mud] += ({ ([ "in transit": 0, "post": tmp ]) }); 
    SERVICES_D->new_mail(mud);
    save_mailqueue(); 
  } 
     
int incoming_post(mapping info) { 
    mapping borg; 
 
    if(file_name(previous_object(0)) != SERVICES_D) return 0;
    info["NAME"] = replace_string(lower_case(info["NAME"]), " ", ".");
    if(info["WIZTO"]) __IncomingMail[info["NAME"]] = info; 
    else { 
        if(!__IncomingMail[info["NAME"]]) return 0; 
        else __IncomingMail[info["NAME"]]["MSG"] += info["MSG"]; 
      } 
    borg = ([]);
    if(info["ENDMSG"]) { 
        borg["to"] = __IncomingMail[info["NAME"]]["WIZTO"]; 
        borg["cc"] = __IncomingMail[info["NAME"]]["CC"]; 
        borg["from"] = __IncomingMail[info["NAME"]]["WIZFROM"]; 
        borg["subject"] = __IncomingMail[info["NAME"]]["SUBJECT"]; 
        borg["message"] = __IncomingMail[info["NAME"]]["MSG"]; 
        borg["date"] = __IncomingMail[info["NAME"]]["DATE"]; 
        LOCALPOST_D->send_post(copy(borg), 
          local_targets(distinct_array(borg["to"] + borg["cc"])));
        map_delete(__IncomingMail, info["NAME"]); 
      } 
    return 1; 
  } 
 
static private string *local_targets(string *str) {
    string a, b; 
    int i;
 
    i = sizeof(str);
    while(i--) {
        sscanf(str[i], "%s@%s", a, b);
        if(replace_string(lower_case(b), " ", ".") == 
          replace_string(lower_case(mud_name()), " ", ".")) str[i] = a;
    }
    return str;
  } 
 
static private void save_mailqueue() { 
    unguarded((: save_object, SAVE_MAILQUEUE :));
  } 
 
static private void restore_mailqueue() { 
    unguarded((: restore_object, SAVE_MAILQUEUE :));
  } 
         
static private string *convert_names(string *noms) {
    string a, b;
    int i;

    i = sizeof(noms);
    while(i--) {
        if(sscanf(noms[i], "%s@%s", a, b) != 2)
          noms[i] = sprintf("%s@%s", noms[i], mud_name());
    }
    return noms;
}

mapping query_mail_queue() { return copy(__MailQueue); }
