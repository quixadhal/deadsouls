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
    string who, ret;
    string adverb = "";
    string machine_message;
    PING_D->SetOK();
    tn("eventReceiveTell: "+identify(packet),"yellow");
    if( file_name(previous_object()) != INTERMUD_D ) return;
    who = convert_name(packet[5]);
    INSTANCES_D->SendTell(who, packet[7], packet[6] + "@" + packet[2]);
    if( !(ob = find_player(who)) || ob->GetInvis() ) {
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
        parse_it=machine->query_answer();
        if(parse_it){
            machine->get_message(packet[6] + "@" + packet[2]+
                    " tells you: "+packet[7]+"\n");
            machine_message=machine->send_message();
            //INTERMUD_D->eventWrite(({ "error", 5, mud_name(), 0, packet[2],
            //    packet[3], "unk-user",
            //    machine_message,
            //    packet }));
            return;
        }
    }
    ret = "%^BOLD%^RED%^" + packet[6] + "@" + packet[2] +
        adverb + " tells you:%^RESET%^ " + packet[7];
    if(member_array(lower_case(packet[6]),ob->GetMuffed()) == -1 &&
            member_array(lower_case(packet[2]),ob->GetMuffed()) == -1){
        if(!machine_message){
            ob->eventPrint(ret, MSG_CONV);
        }
        ob->eventTellHist(ret);
        if(!sizeof(adverb)){
            ob->SetProperty("reply", packet[6] + "@" + packet[2]);
            ob->SetProperty("reply_time", time());
        }
    }
}

void eventSendTell(string who, string where, string msg) {
    string pl, plc, ret;

    pl = this_player(1)->GetName();
    plc = this_player(1)->GetCapName();
    where = INTERMUD_D->GetMudName(where);
    INTERMUD_D->eventWrite(({ "tell", 5, mud_name(), pl, where, 
                convert_name(who), plc, msg }));
    ret = "%^BOLD%^RED%^You tell " + capitalize(who) +
        "@" +  where + ":%^RESET%^ " + msg;
    this_player(1)->eventPrint(ret, MSG_CONV);
    this_player(1)->eventTellHist(ret);
    tn("eventSendTell: "+identify( ({ "tell", 5, mud_name(), pl, where, convert_name(who), plc, msg }) ), "yellow");
}
