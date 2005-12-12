/*    /daemon/services/tell.c
 *    from the Dead Souls LPC Library
 *    handles the IMP tell service
 *    created by Descartes of Borg 950507
 */

#define SERVICE_TELL

#include <daemons.h>
#include <message_class.h>

void eventReceiveTell(mixed *packet) {
    object ob;
    string who;

    if( file_name(previous_object()) != INTERMUD_D ) return;
    who = convert_name(packet[5]);
    if( !(ob = find_player(who)) || (int)ob->GetInvis() ) {
	INTERMUD_D->eventWrite(({ "error", 5, mud_name(), 0, packet[2],
	    packet[3], "unk-user", 
	    capitalize(packet[5]) + " is nowhere to "
	    "be found on " + mud_name() + ".",
	    packet }));
	return;
    }
    ob->eventPrint("%^BOLD%^RED%^" + packet[6] + "@" + packet[2] +
      " tells you:%^RESET%^ " + packet[7], MSG_CONV);
    ob->SetProperty("reply", packet[6] + "@" + packet[2]);
}

void eventSendTell(string who, string where, string msg) {
    string pl, plc;

    pl = (string)this_player(1)->GetName();
    plc = (string)this_player(1)->GetCapName();
    where = (string)INTERMUD_D->GetMudName(where);
    INTERMUD_D->eventWrite(({ "tell", 5, mud_name(), pl, where, 
	convert_name(who), plc, msg }));
    this_player(1)->eventPrint("%^BOLD%^RED%^You tell " + capitalize(who) +
      "@" +  where + ":%^RESET%^ " + msg,
      MSG_CONV);
}




