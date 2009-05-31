/*    /daemon/remotepost.c
 *    from Foundation II
 *    daemon to handle the network to local mail interface
 *    created by Descartes of Borg 940130
 *    modified for IIPS 3.1 940513
 */

#include <lib.h> 
#include <save.h> 
#include <daemons.h> 
#include <objects.h> 
#include "remotepost.h"

inherit LIB_DAEMON;

private mapping __MailQueue; 
static private mapping __IncomingMail; 
mapping Old = ([]);
mapping Outgoing = ([]);
static string SaveFile;

static int count = 0;

void create() {
    string *muds; 
    int i; 
    daemon::create();
    SaveFile = save_file(SAVE_MAILQUEUE);
    SetNoClean(1);
    __MailQueue = ([]); 
    __IncomingMail = ([]); 
    if(file_exists(SaveFile)){
        restore_mailqueue(); 
    }
    i = sizeof(muds = keys(__MailQueue)); 
    while(i--) __MailQueue[muds[i]][0]["in transit"] = 0; 
    set_heart_beat(1);
} 

void heart_beat(){
    count++;
    if(sizeof(Outgoing))
        OOB_D->SendMail(Outgoing);
    Outgoing = ([]);
    if(!(count % 60)){
        if(count > 900){
            count = 0;
            save_mailqueue();
        }
        foreach(mixed key, mixed val in Outgoing){
            if(!sizeof(val)) map_delete(Outgoing,key);
        }
    }
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

int send_post(mapping borg, string mud) {
    string *msg, *tmp, *muds = ({}); 
    int i, maxi, x, y; 
    mapping TmpMap = ([]);

    if(file_name(previous_object(0)) != LOCALPOST_D &&
            base_name(previous_object(0)) != OBJ_POST){
        return 0;
    }

    foreach(mixed val in borg["to"] + borg["cc"]){
        string mudname;
        if(sscanf(val,"%*s@%s",mudname)) {
            mudname = INTERMUD_D->GetMudName(mudname);
            if(mudname){
                OOB_D->RequestToken(mudname);
                muds += ({ mudname });
                if(!TmpMap) TmpMap = ([]);
                if(!TmpMap[mudname]) TmpMap[mudname] = ({});
                TmpMap[mudname] += convert_names(({ val }));
            }
        }
    }

    borg["to"] = convert_names(borg["to"]); 
    borg["cc"] = convert_names(borg["cc"]); 
    borg["from"] = sprintf("%s@%s", convert_name(borg["from"]),
            mud_name());

    foreach(string destination in distinct_array(muds)){
        string *tmp_to = ({});
        string *tmp_cc = ({});

        foreach(string dude in borg["to"]){
            if(member_array(dude,TmpMap[destination]) != -1) tmp_to += ({ dude });
        }
        foreach(string dude in borg["cc"]){
            if(member_array(dude,TmpMap[destination]) != -1) tmp_cc += ({ dude });
        }
        if(!Outgoing) Outgoing = ([]);
        if(!Outgoing[destination]) Outgoing[destination] = ([]);
        Outgoing[destination][borg["id"]] = 
            ({	  
             "mail",
             borg["id"],
             borg["from"],
             ([ destination : tmp_to ]),
             ([ destination : tmp_cc ]),
             ({}),
             borg["id"],
             borg["subject"],
             borg["message"],
             });
    }
    save_mailqueue(); 
    return 1;
} 

int outgoing_sent(string destination, string id){
    if(base_name(previous_object(0)) != LIB_OOB){
        return 0;
    }
    if(sizeof(Outgoing[destination]))
        if(Outgoing[destination][id]) map_delete(Outgoing[destination], id);
    if(!sizeof(Outgoing[destination])) map_delete(Outgoing, destination);
    save_mailqueue();
    return 1;
}

int incoming_post(mixed *packet){ 
    mapping borg; 
    string from = packet[2];
    if(base_name(previous_object(0)) != LIB_OOB){
        return 0;
    }
    if(!grepp(packet[2],"@")) from = packet[2]+"@"+packet[0];
    borg =
        ([
         "id" : packet[1],
         "from" : from,
         "to" : packet[3][mud_name()],
         "cc" : packet[4][mud_name()],
         "date" : ( stringp(packet[6]) ? to_int(packet[6]) : packet[6] ),
         "subject" : packet[7],
         "message" : packet[8]
         ]);
    LOCALPOST_D->send_post(copy(borg)); 
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
    SaveObject(SaveFile);
} 

static private void restore_mailqueue() { 
    RestoreObject(SaveFile);
} 

string *convert_names(string *noms) {
    string a, b;
    string *nombres = ({});

    foreach(string namen in noms){
        if(sscanf(namen, "%s@%s", a, b) != 2) nombres += ({ namen });
        else nombres += ({ a });
    }
    return nombres;
}

mapping query_mail_queue() { return copy(__MailQueue); }

int eventDestruct(){
    save_mailqueue();
    return ::eventDestruct();
}

varargs void defer_old_mail(int i){
    foreach(mixed destination, mixed messages in Outgoing){
        if(!Old) Old = ([]);
        foreach(mixed key, mixed val in messages){
            if(((time() - key) > 300) || i){
                if(!Old[destination]) Old[destination] = ([]);
                if(!Old[destination][key]) Old[destination][key] = val;
                map_delete(Outgoing[destination],key);
            }
        }
    }
}
