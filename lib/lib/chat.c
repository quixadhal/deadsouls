/*    /lib/chat.c
 *    from the Dead Souls LPC Library
 *    chat enabling object
 *    created by Descartes of Borg 950323
 */

#include <lib.h>
#include <daemons.h>
#include "include/chat.h"

private string *RestrictedChannels;
private static mapping Channels;
int NoChanColors = 0;

static void create() {
    RestrictedChannels = ({ });
    Channels = ([ ]);
}

mapping returnChannels(){
    return Channels;
}

static string chat_command(string str) {
    string cmd, arg;
    int x;

    if( (x = strsrch(str, " ")) == -1) {
        cmd = str;
        arg = "";
    }
    else {
        cmd = str[0..(x-1)];
        arg = str[(x+1)..];
    }
    if( cmd == "list" && (int)CHAT_D->cmdChannel(cmd, arg) ) return "";
    else if( Channels[cmd] && (int)CHAT_D->cmdChannel(cmd, arg) ) return "";
    else if( (sscanf(cmd, "%semote", cmd) || sscanf(cmd, "%s:", cmd))
      && Channels[cmd] ) {
        if( (int)CHAT_D->cmdChannel(cmd+"emote", arg) ) return "";
        else return str;
    }
    else return str;
}

static void net_dead() {
    CHAT_D->eventRemoveMember(keys(Channels));
}

void eventReconnect() {
    CHAT_D->eventRegisterMember(keys(Channels));
}

int eventDestruct() {
    if(CHAT_D->eventRemoveMember(keys(Channels))) return 1;
    else return 0;
}

string *AddChannel(mixed val) {
    string *tmp;
    int i, maxi;

    if( stringp(val) ) val = ({ val });
    else if( !pointerp(val) ) error("Bad argument 1 to AddChannel().\n");
    val = val - RestrictedChannels;
    if(arrayp(val)){
        foreach(string channel in val){
            if(this_player()->GetExtraChannels() &&
              member_array(channel,this_player()->GetExtraChannels()) == -1){
                this_player()->AddExtraChannels( ({ channel }) );
            }
        }
    }
    tmp = (string *)CHAT_D->eventRegisterMember(val);
    for(i=0, maxi = sizeof(tmp); i < maxi; i++) Channels[tmp[i]] = 1;
    return keys(Channels);
}

string *RemoveChannel(mixed val) {
    string *tmp;
    int i, maxi;

    if( stringp(val) ) val = ({ val });
    else if( !pointerp(val) ) error("Bad argument 1 to RemoveChannel().\n");
    if(arrayp(val)){
        foreach(string channel in val){
            if(member_array(channel,this_player()->GetExtraChannels()) != -1){
                this_player()->RemoveExtraChannels( ({ channel }) );
            }
        }
    }
    tmp = (string *)CHAT_D->eventRemoveMember(val);
    for(i=0, maxi = sizeof(tmp); i < maxi; i++) map_delete(Channels, tmp[i]);
    return keys(Channels);
}

string *GetChannels() { return keys(Channels); }

int GetNoChanColors(){
    return NoChanColors;
}

int SetNoChanColors(int x){
    if(x) NoChanColors = 1;
    else NoChanColors = 0;
    return NoChanColors;
}

string *RestrictChannel(mixed val) {
    if( stringp(val) ) val = ({ val });
    else if( !pointerp(val) ) error("Bad argument 1 to RestrictChannel().\n");

    // Only allow restriction of channels that are possesed by the object.
    val = filter(val, (:Channels[$1]:));
    RestrictedChannels = distinct_array(RestrictedChannels + val);
    RemoveChannel(val);
    return RestrictedChannels;
}

string *UnrestrictChannel(mixed val) {
    if( stringp(val) ) val = ({ val });
    else if( !pointerp(val) ) error("Bad argument 1 to UnrestrictChannel().\n");

    // Only add channels that where restricted.
    val = filter(val, (:member_array($1, RestrictedChannels) != -1:));
    RestrictedChannels -= val;
    AddChannel(val);
    return RestrictedChannels;
}

string *GetRestrictedChannels() { return (RestrictedChannels + ({})); }
