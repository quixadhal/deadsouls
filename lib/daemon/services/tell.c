/*    /daemon/services/tell.c
 *    from the Dead Souls LPC Library
 *    handles the IMP tell service
 *    created by Descartes of Borg 950507
 */

#define SERVICE_TELL

#include <daemons.h>
#include <message_class.h>

void eventReceiveTell(mixed *packet) {
    object ob, machine;
    string who;
    string adverb = "";

    if( file_name(previous_object()) != INTERMUD_D ) return;
    who = convert_name(packet[5]);
    if( !(ob = find_player(who)) || (int)ob->GetInvis() ) {
	INTERMUD_D->eventWrite(({ "error", 5, mud_name(), 0, packet[2],
	    packet[3], "unk-user", 
	    capitalize(packet[5]) + " is nowhere to "
	    "be found on " + mud_name() + ".",
	    packet }));
	if(!(ob = find_player(who))) return;
	adverb = " %^BOLD%^MAGENTA%^unknowingly%^BOLD%^RED%^";
    }

    machine=present("answering machine",ob);
    if(machine && base_name(machine) == "/secure/obj/machine"){
	int parse_it;
	string machine_message;
	parse_it=machine->query_answer();
	if(parse_it && !(int)ob->GetInvis()){
	    machine->get_message(packet[6] + "@" + packet[2]+
	      " tells you: "+packet[7]+"\n");
	    machine_message=machine->send_message();
	    INTERMUD_D->eventWrite(({ "error", 5, mud_name(), 0, packet[2],
		packet[3], "unk-user",
		machine_message,
		packet }));
	    return;
	}
    }

    ob->eventPrint("%^BOLD%^RED%^" + packet[6] + "@" + packet[2] +
      adverb + " tells you:%^RESET%^ " + packet[7], MSG_CONV);
    if(!sizeof(adverb)) ob->SetProperty("reply", packet[6] + "@" + packet[2]);
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




