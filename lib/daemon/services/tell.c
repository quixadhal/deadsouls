/*    /daemon/services/tell.c
 *    from the Dead Souls LPC Library
 *    handles the IMP tell service
 *    created by Descartes of Borg 950507
 */

#define SERVICE_TELL

#include <daemons.h>
#include <message_class.h>
#ifndef PLAYER_INTERTELL_ALLOWED
#define PLAYER_INTERTELL_ALLOWED 0
#endif

void eventReceiveTell(mixed *packet) {
    object ob, machine;
    string who, ret;
    string adverb = "";
    string machine_message;
    int nopriv;
    PING_D->SetOK();
    tn("eventReceiveTell: "+identify(packet),"black");
    if( file_name(previous_object()) != INTERMUD_D ) return;
    who = convert_name(packet[5]);
    nopriv = (!creatorp(find_player(who)) && !PLAYER_INTERTELL_ALLOWED);
    INSTANCES_D->SendTell(who, packet[7], packet[6] + "@" + packet[2]);
    if( nopriv || (!(ob = find_player(who)) || ob->GetInvis() ||
                (nopriv = (RESTRICTED_INTERMUD && !imud_privp(lower_case(who)))) )) {
        INTERMUD_D->eventWrite(({ "error", 5, mud_name(), 0, packet[2],
                    packet[3], "unk-user", 
                    capitalize(packet[5]) + " is nowhere to "
                    "be found on " + mud_name() + ".",
                    packet }));
        if(!(ob = find_player(who)) || nopriv) return;
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

    // Quixadhal was here
    if(RESTRICTED_INTERMUD) {
        if(!imud_privp(lower_case(pl))) {
            this_player(1)->eventPrint("You lack the power to send tells to other worlds.", MSG_CONV);
            return;
        }
    }
    if(!PLAYER_INTERTELL_ALLOWED && !creatorp(this_player())){ 
        this_player()->eventPrint("You lack the authority to send tells to other worlds.", MSG_CONV);
        return;
    }

    INTERMUD_D->eventWrite(({ "tell", 5, mud_name(), pl, where, 
                convert_name(who), plc, msg }));
    ret = "%^BOLD%^RED%^You tell " + capitalize(who) +
        "@" +  where + ":%^RESET%^ " + msg;
    this_player(1)->eventPrint(ret, MSG_CONV);
    this_player(1)->eventTellHist(ret);
    tn("eventSendTell: "+identify( ({ "tell", 5, mud_name(), pl, where, convert_name(who), plc, msg }) ), "red");
}
