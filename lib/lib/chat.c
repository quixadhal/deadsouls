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
private int NoChanColors = 0;
private mapping GagMutes = ([]);

static void create(){
    RestrictedChannels = ({ });
    Channels = ([ ]);
    GagMutes = (["local_mute":0,"remote_mute":0,"local_gag":0,"remote_gag":0]);
}

mapping returnChannels(){
    return Channels;
}

static string chat_command(string str){
    string cmd, arg, tmp;
    int x;
    if( (x = strsrch(str, " ")) == -1){
        cmd = str;
        arg = "";
    }
    else {
        cmd = str[0..(x-1)];
        arg = str[(x+1)..];
    }
    if( cmd == "list" && CHAT_D->cmdChannel(cmd, arg) ) return "";
    else if( Channels[cmd] && CHAT_D->cmdChannel(cmd, arg) ) return "";
    else if( (sscanf(cmd, "%semote", tmp) || sscanf(cmd, "%s:", tmp)
                || sscanf(cmd, "%s|%*s", tmp))
            && Channels[tmp] ){
        if( CHAT_D->cmdChannel(cmd, arg) ) return "";
        else return str;
    }
    else return str;
}

static void net_dead(){
    CHAT_D->eventRemoveMember(keys(Channels));
}

void eventReconnect(){
    CHAT_D->eventRegisterMember(keys(Channels));
}

int eventDestruct(){
    if(CHAT_D->eventRemoveMember(keys((Channels || ([]))))) return 1;
    else return 0;
}

string *AddChannel(mixed val){
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
    tmp = CHAT_D->eventRegisterMember(val);
    for(i=0, maxi = sizeof(tmp); i < maxi; i++) Channels[tmp[i]] = 1;
    return keys(Channels);
}

string *RemoveChannel(mixed val){
    string *tmp;
    int i, maxi;
    object prev = this_player();

    if( stringp(val) ) val = ({ val });
    if(!prev ||(prev != this_object() && !adminp(prev))){
        return keys(Channels);
    }
    else if( !pointerp(val) ) error("Bad argument 1 to RemoveChannel().\n");
    if(arrayp(val)){
        foreach(string channel in val){
            if(member_array(channel,this_player()->GetExtraChannels()) != -1){
                this_player()->RemoveExtraChannels( ({ channel }) );
            }
        }
    }
    tmp = CHAT_D->eventRemoveMember(val);
    for(i=0, maxi = sizeof(tmp); i < maxi; i++) map_delete(Channels, tmp[i]);
    return keys(Channels);
}

string *GetChannels(){ return keys(Channels); }
int GetChannel(string chan){ 
    return ( member_array(chan, keys(Channels)) != -1 ); 
}

int GetNoChanColors(){
    return NoChanColors;
}

int SetNoChanColors(int x){
    if(x) NoChanColors = 1;
    else NoChanColors = 0;
    return NoChanColors;
}

string *RestrictChannel(mixed val){
    if( stringp(val) ) val = ({ val });
    else if( !pointerp(val) ) error("Bad argument 1 to RestrictChannel().\n");

    // Only allow restriction of channels that are possesed by the object.
    val = filter(val, (:Channels[$1]:));
    RestrictedChannels = distinct_array(RestrictedChannels + val);
    RemoveChannel(val);
    return RestrictedChannels;
}

string *UnrestrictChannel(mixed val){
    if( stringp(val) ) val = ({ val });
    else if( !pointerp(val) ) error("Bad argument 1 to UnrestrictChannel().\n");

    // Only add channels that where restricted.
    val = filter(val, (:member_array($1, RestrictedChannels) != -1:));
    RestrictedChannels -= val;
    AddChannel(val);
    return RestrictedChannels;
}

string *GetRestrictedChannels(){ return (RestrictedChannels + ({})); }

int GetMutedType(string type){
    if(type == "local" && GagMutes["local_mute"]) return 1;
    if(type == "remote" && GagMutes["remote_mute"]) return 1;
    if(type == "all" && GagMutes["remote_mute"] && 
            GagMutes["local_mute"]) return 1;
    return 0;
}

int GetMuted(string channel){
    channel = CHAT_D->GetRemoteChannel(channel);
    if(GagMutes["remote_mute"]
            && member_array(channel, CHAT_D->GetRemoteChannels()) != -1
            && member_array(channel, CHAT_D->GetLocalChannels()) == -1){
        return 1;
    }
    if(GagMutes["local_mute"]
            && ( member_array(channel, CHAT_D->GetRemoteChannels()) == -1
                || member_array(channel, CHAT_D->GetLocalChannels()) != -1)){
        return 1;
    }
    return 0;
}

int GetGaggedType(string type){
    if(type == "local" && GagMutes["local_gag"]) return 1;
    if(type == "remote" && GagMutes["remote_gag"]) return 1;
    if(type == "all" && GagMutes["remote_gag"] && 
            GagMutes["local_gag"]) return 1;
    return 0;
}

int GetGagged(string channel){
    channel = CHAT_D->GetRemoteChannel(channel);
    if(GagMutes["remote_gag"]
            && member_array(channel, CHAT_D->GetRemoteChannels()) != -1
            && member_array(channel, CHAT_D->GetLocalChannels()) == -1){
        return 1;
    }
    if(GagMutes["local_gag"]
            && ( member_array(channel, CHAT_D->GetRemoteChannels()) == -1
                || member_array(channel, CHAT_D->GetLocalChannels()) != -1)){
        return 1;
    }
    return 0;
}

int SetGagged(string type, mixed x){
    object prev = this_player();
    int ret;
    if(!prev || (prev != this_object() && !adminp(prev))) return 0;
    if(undefinedp(x) || !intp(x)) x = 1;
    if(!type) type = "all";
    switch(type){
        case "local" :
            ret = GagMutes["local_gag"];
        if(x > 0) ret = ((ret != 2) ? (adminp(prev) ? 2 : 1) : 2);
        else ret = ((ret == 2) ? (adminp(prev) ? 0 : 2) : 0);
        GagMutes["local_gag"] = ret;
        break;
        case "remote" :
            ret = GagMutes["remote_gag"];
        if(x > 0) ret = ((ret != 2) ? (adminp(prev) ? 2 : 1) : 2);
        else ret = ((ret == 2) ? (adminp(prev) ? 0 : 2) : 0);
        GagMutes["remote_gag"] = ret;
        break;
        case "all" :
            ret = GagMutes["local_gag"];
        ret = (ret > GagMutes["remote_gag"] ? ret : GagMutes["remote_gag"]);
        if(x > 0) ret = ((ret != 2) ? (adminp(prev) ? 2 : 1) : 2);
        else ret = ((ret == 2) ? (adminp(prev) ? 0 : 2) : 0);
        GagMutes["local_gag"] = ret;
        GagMutes["remote_gag"] = ret;
        break;
    }
    return ret;
}

int SetMuted(string type, mixed x){
    object prev = this_player();
    int ret;
    if(!prev || (prev != this_object() && !adminp(prev))) return 0;
    if(undefinedp(x) || !intp(x)) x = 1;
    if(!type) type = "all";
    switch(type){
        case "local" :
            ret = GagMutes["local_mute"];
        if(x > 0) ret = ((ret != 2) ? (adminp(prev) ? 2 : 1) : 2);
        else ret = ((ret == 2) ? (adminp(prev) ? 0 : 2) : 0);
        GagMutes["local_mute"] = ret;
        break;
        case "remote" :
            ret = GagMutes["remote_mute"];
        if(x > 0) ret = ((ret != 2) ? (adminp(prev) ? 2 : 1) : 2);
        else ret = ((ret == 2) ? (adminp(prev) ? 0 : 2) : 0);
        GagMutes["remote_mute"] = ret;
        break;
        case "all" :
            ret=GagMutes["local_mute"];
        ret=(ret > GagMutes["remote_mute"] ? ret : GagMutes["remote_mute"]);
        if(x > 0) ret = ((ret != 2) ? (adminp(prev) ? 2 : 1) : 2);
        else ret = ((ret == 2) ? (adminp(prev) ? 0 : 2) : 0);
        GagMutes["local_mute"] = ret;
        GagMutes["remote_mute"] = ret;
        break;
    }
    return ret;
}
