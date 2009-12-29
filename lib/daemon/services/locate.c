/*    /daemon/services/locate.c
 *    From the Dead Souls Mud Library
 *    performs player lookups
 *    Created by Descartes of Borg 950624
 *    Version: @(#) locate.c 1.2@(#)
 *    Last modified: 96/12/09
 */

#define SERVICE_LOCATE

#include <daemons.h>
#include ROOMS_H
#include <message_class.h>

static mapping locate_user_table = ([]);

static string eventLookupLocateUser(string str){
    if(!locate_user_table) locate_user_table = ([]);
    if(!locate_user_table[str]) return str;
    else {
        string ret = locate_user_table[str];
        //map_delete(locate_user_table, str);
        return ret;
    }
}

void eventReceiveLocateRequest(mixed array packet) {
    string status;
    int idl = 0;
    object ob;
    PING_D->SetOK();
    if( file_name(previous_object()) != INTERMUD_D ) return;
    //tell_room(ROOM_ARCH,"The Arch Room loudspeaker announces: \"%^BOLD%^CYAN%^"+capitalize(packet[3])+" at "+packet[2]+" has issued a locate request for %^BOLD%^YELLOW%^"+capitalize(packet[6])+".%^RESET%^\"");
    tn("Locate request received: "+identify(packet),"white");
    CHAT_D->eventSendChannel("SYSTEM","intermud","[" + capitalize(packet[3])+"@"+packet[2]+
            " seeks "+packet[6]+ "]",0);
    if( !(ob = find_player(packet[6])) || ob->GetInvis()) return;
    if( interactive(ob) ) {
        string array tmp = ({ });
        if( in_edit(ob) || ob->GetCedmode() )
            tmp += ({ "editing" });
        if( (idl = query_idle(ob)) > 60 )
            tmp += ({ "inactive" });
        if( sizeof(tmp) ) status = implode(tmp, ", ");
        else status = 0;
    }
    else status = "link-dead";
    INTERMUD_D->eventWrite( ({ "locate-reply", 5, mud_name(), 0, packet[2], 
                packet[3], mud_name(),
                ob->GetName(), idl, status }) );
}

void eventReceiveLocateReply(mixed array packet) {
    object ob;
    string m;
    int idl;
    PING_D->SetOK();
    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( !packet[5] || !(ob = find_player(convert_name(eventLookupLocateUser(packet[5])))) ){
        return;
    }
    tn("Locate reply received: "+identify(packet),"white");
    m = packet[7] + " was just located on " + packet[6] + ".";
    if( (idl = packet[8]) > 60 )
        m += sprintf(" (idle %02d:%02d:%02d)", idl/3600, (idl/60)%60, idl%60);
    if( stringp(packet[9]) )
        m += " [status: " + packet[9] + "]";
    ob->eventPrint(m, MSG_SYSTEM);
}

void eventSendLocateRequest(string target) {
    string who, crypt_who;
    mixed *locate_request;

    who = this_player(1)->GetKeyName();

    if(this_player(1)->GetInvis()){
        foreach(string key, string val in locate_user_table){
            if(!key || ! val) continue;
            if(val == who){
                crypt_who = key;
            }
        }
        if(!crypt_who){
            crypt_who = alpha_crypt(10);
            locate_user_table[crypt_who] = who;
        }
    }

    else crypt_who = who;

    locate_request = ({ "locate-req", 5, mud_name(), crypt_who, 0, 0, target });
    INTERMUD_D->eventWrite( locate_request );
    tn("Locate request being sent: "+identify(locate_request),"white");
}
