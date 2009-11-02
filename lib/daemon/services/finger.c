/*    /daemon/services/finger.c
 *    From the Dead Souls Mud Library
 *    Intermud 3 finger service implementation
 *    created by Descartes of Borg 950624
 *    Version: @(#) finger.c 1.2@(#)
 *    Last modified: 96/12/14
 */

#define SERVICE_FINGER

#include <daemons.h>
#include ROOMS_H
#include <message_class.h>

void eventReceiveFingerRequest(mixed array packet) {
    mixed array ret;
    PING_D->SetOK();
    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( !(ret = (mixed array)FINGER_D->GetRemoteFinger(packet[6])) ) {
        INTERMUD_D->eventWrite(({ "error", 5, mud_name(), 0, packet[2],
                    packet[3], "unk-user",
                    capitalize(packet[6]) + " is not involved.",
                    packet }));
        return;
    }
    ret = ({ "finger-reply", 5, mud_name(), 0, packet[2], packet[3] }) + ret;
    INTERMUD_D->eventWrite(ret);
    //tell_room(ROOM_ARCH,"The Arch Room loudspeaker announces: \"%^BOLD%^CYAN%^"+capitalize(packet[3])+" at "+packet[2]+" has requested finger information about "+capitalize(ret[6])+".%^RESET%^\"");
    tn("eventReceiveFingerRequest: "+identify(packet),"cyan");
    CHAT_D->eventSendChannel("SYSTEM","intermud","[" + capitalize(packet[3])+"@"+
            packet[2]+" fingers "+capitalize(ret[6])+ "]",0);
}

void eventReceiveFingerReply(mixed array packet) {
    string fing;
    object ob;
    int i;
    PING_D->SetOK();
    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( !(ob = find_player(convert_name(packet[5]))) ) return;
    fing = "Finger information on " + packet[6] + " from " + packet[2] + ":\n";
    if( packet[7] ) fing += replace_string(packet[7], "$N", packet[6]);
    else fing += packet[6];
    if( packet[8] ) fing += " (" + packet[8] + ")\n";
    else fing += "\n";
    fing += "Level: " + (packet[13] ? packet[13] : "Confidential") + "\n";
    fing += "Email: " + (packet[9] ? packet[9] : "Confidential") + "\n";
    if( (i = packet[11]) != -1 ) {
        fing += (packet[10] ? "On since: " + packet[10] : "Online");
        fing += sprintf(" (idle %02d:%02d:%02d)\n", i/3600, (i/60)%60, i%60);
    }
    else fing += (packet[10] ? "Last logged in: " + packet[10] + "\n" :
            "Not logged in.\n");
    fing += "Site: " + (packet[12] ? packet[12] : "Confidential") + "\n";
    fing += (packet[14] ? packet[14] : "\n");
    ob->eventPrint(fing, MSG_SYSTEM);
    tn("eventReceiveFingerReply: "+identify(packet),"cyan");
}

void eventSendFingerRequest(string who, string where) {
    string pl;

    if( !(pl = this_player(1)->GetKeyName()) ) return;
    INTERMUD_D->eventWrite( ({ "finger-req", 5, mud_name(), pl, where, 0, 
                who }) );
    tn("eventSendFingerRequest: "+identify( ({ "finger-req", 5, mud_name(), pl, where, 0,who }) ),"cyan");
}
