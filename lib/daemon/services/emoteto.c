/*    /daemon/services/emoteto.c
 *    from the Foundation II Object Library
 *    handles the I3 emoteto service
 *    created by Descartes of Borg 950914
 */

#define SERVICE_EMOTETO

#include <daemons.h>

void eventReceiveEmote(mixed *packet) {
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
    packet[7] = replace_string(packet[7], "$N", packet[6] + "@" + packet[2]);
    message("emote", packet[7], ob);
}

void eventSendEmote(string who, string where, string msg) {
    string pl, plc;

    pl = (string)this_player(1)->GetKeyName();
    plc = (string)this_player(1)->GetCapName();
    where = (string)INTERMUD_D->GetMudName(where);
    INTERMUD_D->eventWrite(({ "emoteto", 5, mud_name(), pl, where, 
	convert_name(who), plc, msg }));
}




