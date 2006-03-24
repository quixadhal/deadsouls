/*    /daemon/services/who.c
 *    from the Dead Souls LPC Library
 *    handles the Intermud 3 who service
 *    created by Descartes of Borg 950623
 */

#define SERVICE_WHO

#include <daemons.h>
#include <rooms.h>

void eventReceiveWhoReply(mixed *packet) {
    string *list, *who;
    object ob;

    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( !packet[5] || !(ob = find_player(convert_name(packet[5]))) ) return;
    list = ({ "Remote who information from " + packet[2] + ":" });
    foreach(who in packet[6]) 
    list += ({ who[0] + " (" + who[1] + " idle): " + who[2] });
    ob->eventPage(list);
}

void eventReceiveWhoRequest(mixed *packet) {
    mixed *msg;
    string ret = "";
    if( file_name(previous_object()) != INTERMUD_D ) return;
    msg = map(filter(users(), (: (environment($1) && !((int)$1->GetInvis()))  :)),
      (: ({ (string)$1->GetCapName(), query_idle($1),
	  (string)$1->GetShort() }) :));
    INTERMUD_D->eventWrite(({ "who-reply", 5, mud_name(), 0, packet[2],
	packet[3], msg }));
    foreach(string *entry in msg){
	ret += entry[0]+", ";
    }
    ret = truncate(ret,2);

    tell_room(ROOM_ARCH,"The Arch Room loudspeaker announces: \"%^BOLD%^CYAN%^"+capitalize(packet[3])+" at "+packet[2]+" has requested a list of users currently logged on. Replying with: %^BOLD%^YELLOW%^"+ret+".%^RESET%^\"");
}

void eventSendWhoRequest(string mud) {
    string who;

    who = (string)this_player(1)->GetName();
    INTERMUD_D->eventWrite(({ "who-req", 5, mud_name(), who, mud, 0 }));
}




