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

int count = 0;

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
    set_heart_beat(10);
} 

void heart_beat(){
    count++;
    if(sizeof(Outgoing))
	OOB_D->SendMail(copy(Outgoing));
    if(count > 30){
	count = 0;
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

    //if(borg) tc("borg: "+identify(borg),"green");
    //if(mud) tc("mud: "+identify(mud),"white");
    if(file_name(previous_object(0)) != LOCALPOST_D &&
      base_name(previous_object(0)) != OBJ_POST){
	//tc("REMOTEPOST_D.send_post: "+identify(previous_object()),"red");
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

    //tc("borg[\"to\"]: "+identify(borg["to"]),"cyan");
    //tc("borg[\"cc\"]: "+identify(borg["cc"]),"cyan");

    borg["to"] = convert_names(borg["to"]); 
    borg["cc"] = convert_names(borg["cc"]); 
    borg["from"] = sprintf("%s@%s", convert_name(borg["from"]),
      mud_name());

    //tc("borg[\"to\"]: "+identify(borg["to"]),"cyan");
    //tc("borg[\"cc\"]: "+identify(borg["cc"]),"cyan");


    foreach(string destination in singular_array(muds)){
	string *tmp_to = ({});
	string *tmp_cc = ({});

	//tc("TmpMap["+destination+"]: "+identify(TmpMap[destination]),"cyan");

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
	//tc("Outgoing["+destination+"]["+borg["id"]+"]: "+identify(Outgoing[destination][borg["id"]]));
    }
    //tc("muds: "+identify(muds),"cyan");
    //    SERVICES_D->new_mail(mud);
    save_mailqueue(); 
    return 1;
} 

int outgoing_sent(string destination, string id){
    //tc("outgoing_sent: destination: "+destination+", id: "+id,"cyan");
    if(base_name(previous_object(0)) != LIB_OOB){
	//tc("REMOTEPOST_D.outgoing_sent: "+identify(previous_object()),"red");
	return 0;
    }
    map_delete(Outgoing[destination], id);
    if(!sizeof(Outgoing[destination])) map_delete(Outgoing, destination);
    save_mailqueue();
    //tc("Outgoing: "+identify(Outgoing),"blue");
    return 1;
}

int incoming_post(mixed *packet){ 
    mapping borg; 
    string from = packet[2];
    //tc("incoming post: "+identify(packet),"green");
    if(base_name(previous_object(0)) != LIB_OOB){
	//tc("REMOTEPOST_D.incoming_post: "+identify(previous_object()),"red");
	return 0;
    }
    if(!grepp(packet[2],"@")) from = packet[2]+"@"+packet[0];
    borg =
    ([
      "id" : packet[1],
      "from" : from,
      "to" : packet[3][mud_name()],
      "cc" : packet[4][mud_name()],
      "date" : packet[6],
      "subject" : packet[7],
      "message" : packet[8]
    ]);
//tc("INCOMING POST: "+identify(packet),"green");
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
    unguarded((: save_object, SAVE_MAILQUEUE :));
} 

static private void restore_mailqueue() { 
    unguarded((: restore_object, SAVE_MAILQUEUE :));
} string *convert_names(string *noms) {
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

void defer_old_mail(){
    foreach(mixed destination, mixed messages in Outgoing){
	if(!Old) Old = ([]);
	foreach(mixed key, mixed val in messages){
	    //tc("key: "+identify(key));
	    if((time() - key) > 60){
		if(!Old[destination]) Old[destination] = ([]);
		if(!Old[destination][key]) Old[destination][key] = val;
		//tc("Old: "+identify(Old));
		//tc("Outgoing: "+identify(Outgoing));
		map_delete(Outgoing[destination],key);
	    }
	}
    }
    //tc("Old: "+identify(Old));
    //tc("Outgoing: "+identify(Outgoing));
}
