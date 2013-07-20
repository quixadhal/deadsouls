/*    /daemon/intermud.c
 *    from the Dead Souls LPC Library
 *    daemon handling the InterMUD-3 Protocol
 *    created by Descartes of Borg 950506
 *    Version: @(#) intermud.c 1.7@(#)
 *    Last modified: 96/12/14
 */

#ifndef __PACKAGE_SOCKETS__
#error You should not try and load /daemon/intermud.c with no sockets package.
#else

#include <lib.h>
#include <dirs.h>
#include <logs.h>
#include <privs.h>
#include <save.h>
#include <daemons.h>
#include <commands.h>
#include NETWORK_H
#include ROOMS_H
#include "include/intermud.h"

inherit LIB_CLIENT;

static private int Password;
private mapping MudList, ChannelList;
private mapping Banned;
private mixed *Nameservers;
private static int Tries;
private static int SocketStat = -1;
private static int Online = 0;
private static string my_ip;
static string SaveFile;

mapping ExtraInfo();
void ConvertLists();
int GetStatus(string mud);
static mixed SetStatus(string mud, int status);

static void create(){
    client::create();
        Password = SECRETS_D->GetSecret("I3_SERVER_PW");
        SaveFile = save_file(SAVE_INTERMUD);
        tn("INTERMUD_D: prev: "+identify(previous_object(-1)),"red");
        Tries = 0;
        Banned = ([]);
        MudList = ([]);
        ChannelList = ([]);
        MudList["ID"] = -1;
        MudList["List"] = ([]);
        ChannelList["ID"] = -1;
        ChannelList["List"] = ([]);
        if(file_exists(SaveFile)){
            RestoreObject(SaveFile, 1);
        }
    ConvertLists();
    Nameservers = ({ ({ "*dalet", "97.107.133.86 8787" }) });
        SetNoClean(1);
        tn("INTERMUD_D reloaded.");
        tn("Loading object stack: "+get_stack(),"red");
        tn("Loading object trail: "+identify(previous_object(-1)),"red");
        SetDestructOnClose(1);
        SetSocketType(MUD);
        if(DISABLE_INTERMUD == 1){
            call_out( (: eventDestruct :), 2);
        }
        else call_out( (: Setup :), 2);
}

void FirstPing(){
    PING_D->eventPing();
}

static void Setup(){
    string ip;
        int port;
        
        if( !Nameservers || !sizeof(Nameservers) ) return;
            sscanf(Nameservers[0][1], "%s %d", ip, port);
                SocketStat = eventCreateSocket(ip, port);
                if( SocketStat < 0 ) return;
                    tn("INTERMUD_D: SocketStat: "+SocketStat);
                        eventWrite( ({ "startup-req-3", 5, mud_name(), 0, Nameservers[0][0], 0,
                                    Password, MudList["ID"], ChannelList["ID"], query_host_port(),
                                    PORT_OOB, PORT_UDP, mudlib() + " " + mudlib_version(), 
                                    mudlib() + " " + mudlib_version(), version(), "LPMud",
                                    MUD_STATUS, ADMIN_EMAIL,
                                    SERVICES_D->GetServices(), ExtraInfo() }) );
                        tn("INTERMUD_D setup: "+identify( ({
                                        "startup-req-3", 5, mud_name(), 0, Nameservers[0][0], 0,
                                        Password, MudList["ID"], ChannelList["ID"], query_host_port(),
                                        PORT_OOB, PORT_UDP, mudlib() + " " + mudlib_version(),
                                        mudlib() + " " + mudlib_version(), version(), "LPMud",
                                        MUD_STATUS, ADMIN_EMAIL,
                                        SERVICES_D->GetServices(), ([]) }) ), "red");;
                        call_out( (: FirstPing :), 5);
                        call_out( (: load_object :), 5, IMC2_D);
}

int GetConnectedStatus(){
    return Online;
}

void eventClearVars(){
    if( !(master()->valid_apply(({ PRIV_ASSIST, INTERMUD_D }))) )
        error("Illegal attempt to reset intermud: "+get_stack()+" "+identify(previous_object(-1)));
            Tries = 0;
            MudList = ([]);
            ChannelList = ([]);
            MudList["ID"] = -1;
            MudList["List"] = ([]);
            ChannelList["ID"] = -1;
            ChannelList["List"] = ([]);
            SaveObject(SaveFile);
}

static void eventRead(mixed *packet){
    mixed val;
        string cle;
        int mudpacket;
        Online = 1;
        
        if( !packet || sizeof(packet) < 6 ) return; /* should send error */
            if( Banned[packet[2]] ){
                
                    eventWrite(({ "error", 5, mud_name(), 0, packet[2],
                                packet[3], "unk-user", 
                                "Your mud is not allowed to send to "+mud_name()+".",
                                packet }));
                    
                    return;
            }
    switch(packet[0]){
        case "startup-reply":
            LOG_D->LogSpecial(LOG_I3,timestamp()+" "+identify(packet)+"\n");
            tn("INTERMUD_D: "+identify(packet),"red");
            if( sizeof(packet) != 8 ){
                //tn("We don't like the mudlist packet size.","red");
                return;  
            }
        if( !sizeof(packet[6]) ){
            //tn("We don't like an absence of packet element 6.","red");
            return;
        }
        /* Start of Tricky's patch */
            if( packet[2] != Nameservers[0][0] ){
                tn("Illegal startup-reply from mud " + packet[2], "red");
                    return;
            }
        /* End of Tricky's patch */ 
            if( packet[6][0][0] == Nameservers[0][0] ){
                Nameservers = packet[6];
                    Password = packet[7];
                    SECRETS_D->SetSecret("I3_SERVER_PW", Password);
                    SaveObject(SaveFile);
            }
            else {
                Nameservers = packet[6];
                    Setup();
            }
        return;
            case "mudlist":
            tn("INTERMUD_D mudlist received.","red");
            //log_file(LOG_I3,identify(packet),1);
            if( sizeof(packet) != 8 ){
                //tn("We don't like the mudlist packet size.","red");
                return;  
            }
        if( packet[6] == MudList["ID"] )  {
            //tn("We don't like packet element 6. It is: "+identify(packet[6]),"red");
            //tn("We will continue anyway.","red");
        }
        if( packet[2] != Nameservers[0][0] ){
            //tn("We don't like packet element 2. It is: "+identify(packet[2]),"red");
            return;
        }
        
            MudList["ID"] = packet[6];
            foreach(cle, val in packet[7]){
                string tmp = "";
                    if(cle){
                        string lib = "unknown";
                            if(val && sizeof(val) > 5 && arrayp(val)){
                                if(!my_ip && cle == mud_name()) my_ip = val[1];
                                    lib = val[5];
                            }
                        MUDINFO_D->ReceiveMudInfo(cle, val);
                            tmp += "%^BOLD%^CYAN%^Processing mud: "+identify(cle)+
                            ", lib: "+lib;
                            if(val){
                                if(!val[0]) tmp += " %^RED%^BOLD%^offline%^RESET%^";
                                else tmp += " %^GREEN%^BOLD%^ONLINE%^RESET%^";
                            }
                            else tmp += " removed from mudlist";
                                tn(tmp);
                                    //tn(tmp,"cyan",ROOM_ARCH);
                                    if(val && sizeof(val) > 5 && arrayp(val))
                                        tmp = (val[0] ? "%^BLUE%^online" : "%^RED%^offline")+ 
                                            "%^RESET%^, lib: "+lib+", driver: "+val[7];
                                    else tmp = "removed from mudlist.";
                                        CHAT_D->eventSendChannel(cle+"@i3","muds",tmp,0);
                    }
                if( !val && MudList["List"][cle] != 0 ) 
                    map_delete(MudList["List"], cle);
                else if( val ) MudList["List"][cle] = val;
            }
            SaveObject(SaveFile);
            return;
            case "chanlist-reply":
            tn("chanlist reply: "+identify(packet), "blue");
            if( packet[2] != Nameservers[0][0] ) return;
                ChannelList["ID"] = packet[6];
                    foreach(cle, val in packet[7]){
                        if( !val && ChannelList["List"] != 0 ){
                            map_delete(ChannelList["List"], cle);
                                CHAT_D->RemoveRemoteChannel(cle);
                        }
                        else if( val ){
                            ChannelList["List"][cle] = val;
                                CHAT_D->AddRemoteChannel(cle);
                        }
                    }
            SaveObject(SaveFile);
            SERVICES_D->eventRegisterChannels(packet[7]);
            return;
            case "ping-req":
            mudpacket = 1;
            SERVICES_D->eventReceiveAuthRequest(packet);
            break;
            case "ping-reply":
            mudpacket = 1;
            SERVICES_D->eventReceiveAuthReply(packet);
            break;
            case "auth-mud-req":
            mudpacket = 1;
            SERVICES_D->eventReceiveAuthRequest(packet);
            break;
            case "auth-mud-reply":
            mudpacket = 1;
            SERVICES_D->eventReceiveAuthReply(packet);
            break;
            case "channel-t":
            mudpacket = 1;
            SERVICES_D->eventReceiveChannelTargettedEmote(packet);
            break;
            case "channel-e":
            mudpacket = 1;
            SERVICES_D->eventReceiveChannelEmote(packet);
            break;
            case "channel-m":
            mudpacket = 1;
            SERVICES_D->eventReceiveChannelMessage(packet);
            break;
            case "chan-who-reply":
            mudpacket = 1;
            SERVICES_D->eventReceiveChannelWhoReply(packet);
            break;
            case "chan-who-req":
            mudpacket = 1;
            SERVICES_D->eventReceiveChannelWhoRequest(packet);
            break;
            case "chan-user-req":
            mudpacket = 1;
            SERVICES_D->eventReceiveChannelUserRequest(packet);
            break;
            case "emoteto":
            mudpacket = 1;
            SERVICES_D->eventReceiveEmote(packet);
            break;
            case "finger-req":
            mudpacket = 1;
            SERVICES_D->eventReceiveFingerRequest(packet);
            break;
            case "finger-reply":
            mudpacket = 1;
            SERVICES_D->eventReceiveFingerReply(packet);
            break;
            case "locate-req":
            mudpacket = 1;
            SERVICES_D->eventReceiveLocateRequest(packet);
            break;
            case "locate-reply":
            mudpacket = 1;
            SERVICES_D->eventReceiveLocateReply(packet);
            break;
            case "tell":
            mudpacket = 1;
            SERVICES_D->eventReceiveTell(packet);
            break;
            case "chan-user-reply":
            mudpacket = 1;
            tn("INTERMUD_D: chan-user-reply received.","red");
            case "ucache-update":
            mudpacket = 1;
            SERVICES_D->eventReceiveUcacheUpdate(packet);
            break;
            case "who-req":
            mudpacket = 1;
            SERVICES_D->eventReceiveWhoRequest(packet);
            break;
            case "who-reply":
            mudpacket = 1;
            SERVICES_D->eventReceiveWhoReply(packet);
            break;
            case "news":
            mudpacket = 1;
            SERVICES_D->eventReceiveNews(packet);
            break;
            case "mail":
            mudpacket = 1;
            SERVICES_D->eventReceiveMail(packet);
            break;
            case "mail-ok":
            mudpacket = 1;
            SERVICES_D->eventReceiveMailOk(packet);
            break;
            case "file":
            mudpacket = 1;
            tn("INTERMUD_D: file packet received.","red");
            break;
            case "error":
            SERVICES_D->eventReceiveError(packet);
            break;
        default:
                break;
    }
    if(mudpacket){
        if(!GetStatus(packet[2])){
            tn("Setting status of "+packet[2]+" to online.");
            SetStatus(packet[2], -1);
        }
    }
}

static void eventSocketClose(){
    Online = 0;
}

static void eventConnectionFailure(){
    Online = 0;
        tn("INTERMUD_D: CONNECTION FAILED","red");
        error("Failed to find a useful name server.\n");
}

int SetDestructOnClose(int x){ return 0; }

static void eventClose(mixed arg){
    SocketStat = -1;
        Online = 0;
        tn("INTERMUD_D: socket closing!");
        ::eventClose(arg);
}

int SetSocketType(int x){ return client::SetSocketType(MUD); }

string GetMudName(string mud){
    string *lc, *uc;
        mapping mudses = GetMudList();
        int x;
        
        if( MudList["List"][mud] ) return mud;
            lc = map(uc = keys(MudList["List"]), function(string str){
                    if( !str ) return "";
                    else return lower_case(str);
                    });
    x = member_array(lower_case(mud), lc);
        if( x < 0 ){
            foreach(string name in GetMuds()){
                if(mudses[name][1] +" "+mudses[name][2] == mud) return name;
            }
            return 0;
        }
        else return uc[x];
}

static mixed SetStatus(string mud, int status){
    mapping mlist = copy(MudList["List"]);
    mixed array newmud = ({});
    if(!sizeof(newmud = mlist[mud])) return 0;
    tn("old: "+identify(newmud));
    newmud[0] = status;
    MudList["List"][mud] = newmud;
    tn("new: "+identify(MudList["List"][mud]),"blue");
} 

int GetStatus(string mud){
    mixed array newmud = ({});
    if(!sizeof(newmud = MudList["List"][mud])) return 0;
    return (newmud[0] ? 1 : 0);
}

mapping GetMudList(){ 
    if(sizeof(MudList) && MudList["List"])
        return copy(MudList["List"]); 
    else return ([]);
}

string *GetMuds(){
    if(sizeof(MudList) && MudList["List"])
        return keys(MudList["List"]);
    else return ({});
}

string *GetLCMuds(){
    string *orig_arr, *new_arr;
        orig_arr = GetMuds();
        new_arr = ({});
        foreach(string namen in orig_arr){
            new_arr += ({ lower_case(namen) });
        }
    return new_arr;
}

mapping GetChannelList(){ return copy(ChannelList["List"]); }

string *GetChannels(){ return keys(ChannelList["List"]); }

string *GetMatch(string mud){
    string *uc, *lc;
        
        mud = lower_case(mud);
        lc = map(uc = keys(MudList["List"]), (: lower_case :));
        return map(filter(regexp(lc, "^"+mud, 1), (: intp :)), (: $(uc)[$1] :));
}

string GetNameserver(){ return Nameservers[0][0]; }

mixed *GetNameservers(){ return copy(Nameservers); }

int AddBanned(string mud, string reason){
    if( !master()->valid_apply(({})) ){
        return 0;
    }
    if( !(mud = GetMudName(mud)) ){
        return 0;
    }
    Banned[mud] = reason;
        SaveObject(SaveFile);
        return 1;
}

mapping GetBanned(){ return copy(Banned); }

int RawSend(string *packet){
    if(!this_player() || !archp(this_player())) return 0;
        eventWrite(packet);
            return 1;
}

string nextboot(){
    string str;
        int x, offset;
        
        if(DISABLE_REBOOTS){
            return "never";
        }
    
        offset = TIME_D->GetOffset(local_time()[9]);
        offset += EXTRA_TIME_OFFSET;
        
        x = EVENTS_D->GetRebootInterval() * 3600;
        x = (time() - uptime()) + x;
        if(!LOCAL_TIME) 
            x += offset * 3600;
                str = query_tz()+ " " + ctime(x);
                return str;
}

//This new packet element data added to be able
//to handle liveupgrade stuff more sensibly.
mapping ExtraInfo(){
    return ([
            "native version" : native_version(),
            "os build" : query_os_type(),
            "architecture" : architecture(),
            "upsince" : ctime(time() - uptime()),
            "next boot" : nextboot(),
            "ip" : HOST_IP,
            "oob port" : PORT_OOB,
            ]);
}

void ConvertLists(){
    if(classp(MudList)){
        mapping TmpMap = ([]);
            MudList = TmpMap;
            MudList["ID"] = -1;
            MudList["List"] = ([]);
            SaveObject(SaveFile);
    }
    if(classp(ChannelList)){
        mapping TmpMap = ([]);
            ChannelList = TmpMap;
            ChannelList["ID"] = -1;
            ChannelList["List"] = ([]);
            SaveObject(SaveFile);
    }
}

int GetEnabled(){
    return !(DISABLE_INTERMUD);
}

string GetMyIp(){
    string ret = "127.0.0.1";
        if(my_ip) ret = my_ip;
            return ret;
}

void eventWrite(mixed val){
    object prev = previous_object();
        if(!prev || prev == this_object() 
                || prev == find_object(PING_D)
                || prev == find_object(SERVICES_D) 
                || prev == find_object(CMD_CHANBAN)
                || prev == find_object(CMD_CHANCREATE) 
                || prev == find_object(CMD_CHANREMOVE) 
                || prev == find_object(CMD_CHANUNBAN)){
            ::eventWrite(val);
        }
}

static void eventSocketError(string str, int x) {
    //debug_message(timestamp()+" i3 "+socket_error(x) + "\n");
    if(query_os_type() == "windows" && grepp(socket_error(x), "Problem with connect")){
        load_object("/secure/cmds/admins/mudconfig")->cmd("intermud disable");
    }
}
#endif /* __PACKAGE_SOCKETS__ */
