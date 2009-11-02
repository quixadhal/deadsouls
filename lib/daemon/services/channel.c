/*    /daemon/services/channel.c
 *    from the Dead Souls LPC Library
 *    handles intermud channels using the Intermud 3 protocols
 *    created by Descartes of Borg 950625
 *    Version: @(#) channel.c 1.2@(#)
 *    Last modified: 96/12/14
 */

#define SERVICE_CHANNEL

#include <daemons.h>
#include ROOMS_H
#include <message_class.h>

static private string *local_chans = ({"newbie","cre","gossip","admin","error",
        "priest", "mage", "explorer", "thief", "fighter", "death" });


void eventReceiveChannelWhoReply(mixed array packet) {
    object ob;

    PING_D->SetOK();
    tn("eventReceiveChannelWhoReply: "+identify(packet),"green");

    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( !(ob = find_player(packet[5])) ) return;
    packet[6] = CHAT_D->GetLocalChannel(packet[6]);
    if( !sizeof(packet[7]) ) {
        ob->eventPrint("No one is listening to " + packet[6] + " at " +
                packet[2] + ".", MSG_SYSTEM);
        return;
    }
    ob->eventPrint("Listening to " + packet[6] + " at " + packet[2] + ":" +
            implode(packet[7], "   "), MSG_SYSTEM);
}

void eventReceiveChannelWhoRequest(mixed array packet) {
    string array who;
    string ret = "";
    PING_D->SetOK();
    if( file_name(previous_object()) != INTERMUD_D ) return;
    who = CHAT_D->GetChannelList(packet[6]);
    INTERMUD_D->eventWrite(({ "chan-who-reply", 5, mud_name(), 0, packet[2],
                packet[3], packet[6], who }));

    foreach(string entry in who){
        ret += entry+", ";
    }
    ret = truncate(ret,2);
    tn("eventReceiveChannelWhoRequest: "+identify(packet),"green");

    tell_room(ROOM_ARCH,"The Arch Room loudspeaker announces: \"%^BOLD%^CYAN%^"+capitalize(packet[3])+" at "+packet[2]+" has requested a list of users listening to channel "+packet[6]+". Replying with: %^BOLD%^GREEN%^"+ret+".%^RESET%^\"");
}

void eventReceiveChannelUserRequest(mixed array packet) {
    object ob;
    string visname;
    int gender;
    PING_D->SetOK();
    if( file_name(previous_object()) != INTERMUD_D ) return;
    tn("eventReceiveChannelUserRequest: "+identify(packet),"green");
    if( !(ob = find_player(packet[6])) ) {
        INTERMUD_D->eventWrite( ({ "error", 5, mud_name(), 0, packet[2], 0,
                    "unk-user", packet[6] + " is not a valid "
                    "player.", packet }) );
        return;
    }
    visname = ob->GetCapName();
    switch( ob->GetGender() ) {
        case "male": gender = 0; break;
        case "female": gender = 1; break;
        default: gender = 2; break;
    }
    INTERMUD_D->eventWrite( ({ "chan-user-reply", 5, mud_name(), 0, 
                packet[2], 0, packet[6], visname, gender }));
}

void eventReceiveChannelMessage(mixed array packet) {
    PING_D->SetOK();
    tn("eventReceiveChannelMessage: "+identify(packet),"green");

    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( packet[2] == mud_name() ) return;

    CHAT_D->eventSendChannel(packet[7] + "@" + packet[2], packet[6],
            packet[8]);
    //if(packet[2] != mud_name()) CHAT_D->eventAddLast(packet[6],"",packet[6],packet[8],packet[7] + "@" + packet[2]);

}

void eventReceiveChannelEmote(mixed array packet) {
    PING_D->SetOK();
    tn("eventReceiveChannelEmote: "+identify(packet),"green");

    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( packet[2] == mud_name() ) return;
    if( !packet[7] ) return;
    CHAT_D->eventSendChannel(packet[7] + "@" + packet[2], packet[6],
            packet[8], 1, 0, 0);
    //if(packet[2] != mud_name()) CHAT_D->eventAddLast(packet[6],"",packet[6],packet[7] + "@" + packet[2] + replace_string(packet[8],"$N",""));
}

void eventReceiveChannelTargettedEmote(mixed array packet) {
    string target;
    PING_D->SetOK();
    tn("eventReceiveChannelTargettedEmote: "+identify(packet),"green");

    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( packet[2] == mud_name() ) return;
    if( packet[7] != mud_name() ) target = packet[12] + "@" + packet[7];
    else target = packet[12];
    CHAT_D->eventSendChannel(packet[11] + "@" + packet[2], packet[6],
            packet[9], 1, target, packet[10]);
    if(packet[2] != mud_name()) true();
}

varargs void eventSendChannel(string who, string ch, string msg, int emote,
        string target, string targmsg) {
    mixed array packet;
    mixed array packet_thing = ({ who, ch, msg, emote || "", target || "", targmsg || "" });

    string targpl, where;  // targpl is target keyname

    tn("eventSendChannel raw: "+identify( packet_thing ),"green");

    if( emote ) {
        if( target && targmsg ) {
            if( sscanf(target, "%s@%s", targpl, where) != 2 ) {
                targpl = convert_name(target);
                where = mud_name();
            }
            else {
                where = trim(where);
                if(!alphap(last(where,1))) where = truncate(where,1);
                if(member_array(lower_case(where), INTERMUD_D->GetLCMuds()) == -1) {
                    write("No such mud.");
                    return;
                }
                target = SERVICES_D->GetRemoteDisplayName(targpl, where);
                if( !target ) target = capitalize(targpl);
            }
            packet = ({ "channel-t", 5, mud_name(), convert_name(who), 0, 0,
                    ch, where, targpl, msg, targmsg, who, target });
        }
        else packet = ({ "channel-e", 5, mud_name(), convert_name(who), 0, 0,
                ch, who, msg });
    }
    else packet = ({ "channel-m", 5, mud_name(), convert_name(who), 0, 0, ch, 
            who, msg });
    if(member_array(ch, local_chans) == -1){
        INTERMUD_D->eventWrite(packet);
        tn("eventSendChannel processed: "+identify(packet),"green");
    }
}

void eventSendChannelWhoRequest(string channel, string mud) {
    string pl;

    pl = this_player(1)->GetKeyName();
    INTERMUD_D->eventWrite(({ "chan-who-req", 5, mud_name(), pl, mud, 0,
                channel }));
    tn("eventSendChannelWhoRequest: "+identify( ({ "chan-who-req", 5, mud_name(), pl, mud, 0, channel })) , "green");
}

void eventRegisterChannels(mapping list) {
    mixed array val;
    string channel, ns;

    if( file_name(previous_object()) != INTERMUD_D ) return;
    ns = INTERMUD_D->GetNameserver();
    foreach(channel, val in list) {
        if( !val ) continue;
        if( member_array(channel, CHAT_D->GetLocalChannels()) == -1){
            INTERMUD_D->eventWrite(({ "channel-listen", 5, mud_name(), 0, ns, 
                        0, channel, 1 }));
            //log_file("channels", "New channel: " + channel + " recognized " +
            //  ctime(time()) + "\nValue: " + identify(val) + "\n\n");
        }
        else INTERMUD_D->eventWrite(({ "channel-listen", 5, mud_name(), 0, ns,
                    0, channel, 0 }));
    }
    tn("eventRegisterChannels: "+identify(list),"green");

}

int eventAdministerChannel(string channel, string array additions,
        string array subs) {
    tn("eventAdministerChannel. ","green");

    if( !(master()->valid_apply( ({}) )) ) return 0;
    if( member_array(channel, INTERMUD_D->GetChannels()) == -1 )
        return 0;
    INTERMUD_D->eventWrite(({ "channel-admin", 5, mud_name(),
                this_player(1)->GetKeyName(),
                INTERMUD_D->GetNameserver(),
                0, channel, additions, subs }));

    tn("eventAdministerChannel: "+channel+" "+identify(additions)+" "+identify(subs),"green");
    return 1;
}

int AddChannel(string channel, int privee) {
    tn("eventAdministerChannel: "+channel+", "+privee,"green");

    if( !(master()->valid_apply( ({}) )) ){ 
        return 0;
    }
    if( member_array(channel, INTERMUD_D->GetChannels()) != -1 ){
        return 0;
    }
    INTERMUD_D->eventWrite(({ "channel-add", 5, mud_name(), 
                this_player(1)->GetKeyName(),
                INTERMUD_D->GetNameserver(), 0,
                channel, privee }));
    return 1;
}

int RemoveChannel(string channel) {
    tn("RemoveChannel: "+identify(channel),"green");

    if( member_array(channel, INTERMUD_D->GetChannels()) == -1 ){
        return 0;
    }
    INTERMUD_D->eventWrite(({ "channel-remove", 5, mud_name(),
                this_player(1)->GetKeyName(),
                INTERMUD_D->GetNameserver(), 0,
                channel }));
    return 1;
}
