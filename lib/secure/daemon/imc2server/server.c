#include <lib.h>
#include <daemons.h>
#include <commands.h>
#include <save.h>
#include "./server_log.h"

#define CALL_OUT_COMPLETE_STARTUP 1

inherit LIB_DAEMON;
static string router_port = "8888";
mapping mudinfo;
static int lastel = 40;

object cmd = find_object(CMD_IMC_SERVER_D);
object rsocket = find_object(RSOCKET_D);
object ssocket = find_object(SSOCKET_D);
object router = find_object(ROUTER_D);

static void validate(){
    if( previous_object() != cmd && previous_object() != rsocket &&
            previous_object() != this_object()  &&
            previous_object() != ssocket && previous_object() != router &&
            !((int)master()->valid_apply(({ "ASSIST" }))) ){
        trr("SECURITY ALERT: validation failure in IMC2_SERVER_D.","red");
        error("Illegal attempt to access IMC2 server daemon: "+get_stack()+
                " "+identify(previous_object(-1)));
    }
}

void create(){
    ::create();
    if(object_file(SAVE_IMC2_SERVER)){
        RestoreObject(SAVE_IMC2_SERVER, 1);
    }
}

string clean_str(string str){ 
    while(sizeof(str) && str[<1] < 32){
        str = str[0..<2];           
    }           
    return str;       
}

mapping query_mudinfo(){
    return copy(mudinfo);
}

string imc2_name(string str){
    return replace_string(str," ","_");
}

string i3_name(string str){
    string ret = replace_string((str || ""),"_"," ");
    if(ROUTER_D->query_mudinfo()[str]) return str; 
    if(ROUTER_D->query_mudinfo()[ret]) return ret; 
    return str;
}

int is_lpmud(string mud){
    string family, truename = i3_name(mud);
    mapping info = ROUTER_D->query_mudinfo()[truename];
    if(!info) return 0;
    if(stringp(info["mud_type"]) && grepp(info["mud_type"],"LP")){
        return 1;
    }
    if(info["other_data"] && stringp(info["other_data"]["versionid"])){
        if(!strsrch(info["other_data"]["versionid"],"Tim's LPC")){
            return 1;
        }
    }
    return 0;
}

string unescape(string str){
    string a,b=str,output="";
    while(sscanf(b,"%s\\%s",a,b)==2){
        output += a;
        if(sizeof(b)){
            switch(b[0]){
                case 34 : output += "\""; break; // '\"' makes warnings.
                case '\\' : output += "\\"; break;
                case 'n' : output += "\n"; break;
                case 'r' : output += "\r"; break;
            }
        }
        b=b[1..];
    }
    output += b;
    output = replace_string(output,"\n\r","\n");
    output = replace_string(output,"\r\n","\n");
    if((sizeof(explode(output," "))==1) && sscanf(output,"\\\"%*s\\\"") )
        output = "\""+output+"\"";
    return output;
}

mapping string_to_mapping(string str){
    // Picks first element off of string and then repeats?
    mapping out=([]);
    int i;
    string what,data,rest;

    rest = str;

    while(sizeof(rest)>0){
        sscanf(rest, "%s=%s", what, rest);
        /*
           write("what="+what+", rest="+rest+"\n");
         */
        // At this point, what is the key, rest is value plus rest.
        if(rest[0]==34){ // value is in quotes, tons of fun!
            // find first quote without a backslash in front?
            /*
               write("rest begings with a quote\n");
             */
            i = 1;
            while(((rest[i]!=34) || (rest[i-1]==92)) && (i<sizeof(rest))){ // 34 = ", 92 = \
                // While this is not a quote, or if this is an escaped quote, keep looking.
                i++;
            }
            // now are 1 space past quote
            data=rest[1..(i-1)]; // skip opening and closing quotes
            rest=rest[(i+2)..]; // skip past space
            // Data is now what was in the quotes... now to un-escape the
            //data...
            out[what]=unescape(data);
        }
        else{ // value is not in quotes, tons of actual non-sarcastic fun!                  // just split it at the first space
            if(sscanf(rest,"%s %s",data,rest)!=2){ // break at first space                      data = rest;
                rest = "";
            }
            if( !data || ((sscanf(data,"%d",i)==1) 
                        && (sprintf("%d",i)==data)))//just number
                out[what]=i;
            else // not just a number
                out[what]=data;
        }
    }
    return out;
}

mixed packetize(string str){
    mixed *ret;
    mixed src, targ, who, whom;
    string tmp, seq, route, type, data;
    int i = sscanf(str,"%s %s %s %s %s %s", src, seq, route, type, targ, data);
    if(i < 6){
        return 0;
    }
    if( (i = sscanf(src,"%s@%s",who, tmp)) == 2) src = tmp;
    if( (i = sscanf(targ,"%s@%s",whom, tmp)) == 2) targ = tmp;
    if(src && src == "*") src = 0;
    if(targ && targ == "*") targ = 0;
    if(who && who == "*") who = 0;
    if(who && who == "*") who = 0;
    if(whom && whom == "*") whom = 0;
    ret = ({ type, 5, src, who, i3_name(targ), whom, data });
    return ret;
}    

mapping parse_info(string str){
    string url, versionid, host;
    int port;
    mapping ret = ([]);
    str = " " + str + " ";
    if(sscanf(str,"%*surl=%s ", url)) ret["url"] = url;
    else if(sscanf(str,"%*surl=%s\n%*s", url)) ret["url"] = url;
    else if(sscanf(str,"%*surl=%s %*s", url)) ret["url"] = url;
    if(sscanf(str,"%*sversionid=\"%s\"%*s", versionid)) 
        ret["versionid"] = versionid;
    else if(sscanf(str,"%*sversionid=%s ", versionid)) 
        ret["versionid"] = versionid;
    else if(sscanf(str,"%*sversionid=%s %*s", versionid))
        ret["versionid"] = versionid;
    if(sscanf(str,"%*shost=\"%s\"%*s", host)) ret["host"] = host;
    else if(sscanf(str,"%*shost=%s ", host)) ret["host"] = host;
    else if(sscanf(str,"%*shost=%s %*s", host)) ret["host"] = host;
    if(sscanf(str,"%*sport=%d ", port)) ret["port"] = port;
    else if(sscanf(str,"%*sport=%d %*s", port)) ret["port"] = port;
    if(url) ret["url"] = url;
    if(versionid) ret["versionid"] = versionid;
    if(host) ret["host"] = host;
    if(port) ret["port"] = port;
    return ret;
}

mixed translate_packet(mixed data, int fd){
    string targ = ROUTER_D->query_connected_fds()[fd];
    mapping i3info, tmpinfo;
    mixed ret = ({});
    string router_name = ROUTER_D->GetRouterName();
    if(!mudinfo) mudinfo = ([]);
    tmpinfo = copy(mudinfo);
    if(stringp(data)) return data;
    if(!arrayp(data) || sizeof(data) < 6) return 0;
    if(data[0] == "mudlist"){
        mapping tmpret = data[7];
        foreach(mixed key, mixed val in tmpret){
            if(!key) continue;
            if(sizeof(val) && val[0] > -1 ){
                ret += ({ "*@" + router_name + " " + time() + " " +
                        router_name + " close-notify *@* host=" + imc2_name(key) });
            }
            else {
                if(!tmpinfo[key]){
                    i3info = ROUTER_D->query_mudinfo()[key];
                    if(!sizeof(i3info)) continue;
                    tmpinfo[key] = ([]);
                    tmpinfo[key]["versionid"] = i3info["mudlib"];
                    tmpinfo[key]["url"] = "http://"+i3info["ip"];
                    tmpinfo[key]["host"] = i3info["ip"];
                    tmpinfo[key]["port"] = i3info["player_port"];
                }
                ret += ({ "*@"+imc2_name(key)+" "+time()+" "+imc2_mud_name()+
                        "!"+router_name+
                        " is-alive *@"+ targ + " versionid=\""+
                        tmpinfo[key]["versionid"]+
                        "\" networkname=\"LPMuds.net\" url=\""+
                        tmpinfo[key]["url"]+"\""+
                        " sha256=0"+
                        " host=\""+tmpinfo[key]["host"]+"\" "+
                        "port="+tmpinfo[key]["port"]});
            }
        }
    }
    if(data[0] == "chanlist-reply"){
        foreach(mixed foo in data){
            string hub = data[2];
            string rname = "LPMuds.net";
            mapping chans = data[7];
            foreach(string chan, mixed arr in chans){
                string owner = replace_string(arr[0]," ","_");
                int priv = arr[1];
                if(!priv) ROUTER_D->AddIMC2Chan(fd, data[4], chan);
                ret += ({
                        "ICE@"+hub+" "+time()+" "+hub+" ice-update *@"+
                        data[4]+" channel="+hub+":"+chan+" owner=admin@"+owner+
                        " operators=admin@"+owner+" policy="+
                        (priv ? "private invited=" : "open excluded=")+
                        "\"a@b\" level=Imm localname="+chan });
            }
        }
    }
    if(data[0] == "tell"){
        string tmp = replace_string(data[7],"\"","'");
        ret = data[3]+"@"+imc2_name(data[2])+" "+time()+" "+
            imc2_name(data[2])+"!"+router_name+" tell "+
            data[5]+"@"+data[4]+" text=\""+unescape(tmp)+"\"";
    }
    if(data[0][0..7] == "channel-"){
        string tmp = replace_string(data[8],"\"","'");
        string emt = "";
        if(data[0][8..8] == "e"){
            tmp = trim(replace_string(data[8],"$N",""));
            emt = " emote=1";
        }
        ret = data[7]+"@"+imc2_name(data[2])+" "+time()+" "+
            imc2_name(data[2])+ "!"+router_name+
            " ice-msg-b *@* channel="+router_name+
            ":"+data[6]+" text=\""+unescape(tmp)+"\""+emt;
    }
    if(data[0] == "who-req"){
        ret = data[3]+"@"+imc2_name(data[2])+" "+time()+" "+
            imc2_name(data[2])+" who *@"+imc2_name(data[4])+
            " type=who";
    }
    if(data[0] == "who-reply"){
        string who_ret = "Who list for "+data[2]+"\n";
        foreach(mixed element in data[6]){
            who_ret += element[0] + " idle: "+
                time_elapsed(element[1])+"\n";
        }
        who_ret = replace_string(who_ret,"\"","'");
        who_ret = replace_string(who_ret,"\r","");
        if(!is_lpmud(data[4])){
            who_ret = replace_string(who_ret,"\n","\\n");
        }
        ret = "*@"+imc2_name(data[2])+" "+time()+" "+
            imc2_name(data[2])+" who-reply "+ capitalize(data[5])+
            "@"+imc2_name(data[4])+" text=\""+ who_ret+"\"";
    }
    if(sizeof(ret)) return ret;
    return 0;
}

varargs void write_data(int fd, mixed data, int startack, float vers){
    mixed *sstat = socket_status(fd);
    object ssock = find_object(SSOCKET_D);
    mapping minfo = ROUTER_D->query_mudinfo();
    mixed ret;
    string targetmud;
    validate();
    reset_eval_cost();
    targetmud = ROUTER_D->query_connected_fds()[fd];
    //tc("write_data to "+targetmud+": "+identify(data)[0..63]+"...","white");
    if(!startack){
        if(!sizeof(minfo) || !sizeof(targetmud)) return;
        if(minfo[targetmud]["disconnect_time"]) return;
    }
    if(arrayp(data)){
        if(!sizeof(data)) return;
        if(!strsrch(data[0],"PW ")) tc("good","green");
        else if(!(ret = translate_packet(data, fd))){
            if(data[0] == "mudlist") return;
            tc("failed to translate: "+identify(data),"red");
            return;
        }
    }
    if(startack) tc("writing to "+targetmud+": "+identify(ret), "yellow");
    if(ret) data = ret;
    if(!arrayp(data)) data = ({ data });
    if(!sstat || sstat[1] != "DATA_XFER" || !ssock || sstat[5] != ssock) return;
    if(member_array(fd, keys(ROUTER_D->query_irn_sockets())) == -1){ 
        int startup;
        string lfcr = "";
        if(sizeof(data) == 1){
            if(!strsrch(data[0], "PW ")){
                tc("startup: "+data[0],"green");
                startup = 1;
            }
        }
        if(sizeof(data) < 6){
            int spew;
            if(sizeof(data) > 2){
                //tc("data packet size: "+sizeof(data));
                spew = 1;
            }
            foreach(mixed element in data){
                float vv;
                int proper = 0;
                if(!sizeof(element)) continue;
                if(vers && vers > 2) proper = 1;
                else {
                    if(minfo[targetmud] && minfo[targetmud]["other_data"] &&
                            vv = minfo[targetmud]["other_data"]["imc_version"]){
                        if(vv > 2) proper = 1;
                    }
                }
                if(startup){
                    if(proper) tc(fd+" "+(vv || vers),"white");
                    else tc("%^B_WHITE%^"+fd+" "+(vv || vers),"black");
                }
                if(proper){
                    element = clean_str(element)+"\r\n";
                }
                else{
                    element = clean_str(element)+"\n\r";
                }
                if(spew){
                    lfcr = replace_string(element,"\n","(LF)");
                    lfcr = replace_string(lfcr,"\r","(CR)");
                    tc("writing to "+fd+": "+identify(lfcr)+"\n","yellow");
                }
                SSOCKET_D->write_data(fd, element);
            }
        }
        else {
            int delay = 1;
            ret = data[0..lastel];
            data = data[lastel+1..];
            while(sizeof(ret)){
                mixed cargo = ret;
                //tc("cargo: "+identify(cargo)+"\n","red");
                call_out("delayed_write", delay, cargo, fd, targetmud);
                ret = data[0..lastel];
                data = data[lastel+1..];
                delay++;
            }
        }
    }
}

static void delayed_write(mixed data, int fd, string targetmud){
    string checkmud = ROUTER_D->query_connected_fds()[fd];
    if(checkmud != targetmud) return;
    foreach(mixed element in data){
        write_data(fd, element);
    }
}

static void close_connection(int fd){
    SSOCKET_D->close_connection(fd);
}

string GetRouterPort(){
    validate();
    return router_port;
}

string SetRouterPort(string str){
    validate();
    router_port = str;
    server_log("Setting router port to: "+str);
    this_object()->SetList();
    return router_port;
}

int eventDestruct(){
    validate();
    SaveObject(SAVE_IMC2_SERVER);
    server_log("I am being destructed by: \n"+get_stack()+
            "\n"+identify(previous_object(-1)));
    daemon::eventDestruct();
}

int GetMaxRetries(){
    return 5;
}

varargs void construct_startup(mixed fd, mixed info, string client){
    string router = ROUTER_D->GetRouterName();
    string s1, s2, s3, s4, s5;
    mixed packet;
    float vers;
    int passwd, pport, scan, newmud;
    mapping other = ([]);
    string *nix = ({ "clientpass", "serverpass", "password" });
    if(!mudinfo) mudinfo = ([]);
    trr("construct_startup hit");
    //if(grepp(info,"\n")) tc("OH YEAH");
    //else tc("bummer.");
    //if(grepp(info,"\r")) tc("WOOT");
    //else tc("lame.");
    scan = sscanf(info,"PW %s %s %s %s %s", s1, s2, s3, s4, s5);
    if(scan != 5) scan = sscanf(info,"PW %s %s %s %s", s1, s2, s3, s4);
    if(scan < 4) scan = sscanf(info,"PW %s %s %s", s1, s2, s3);
    trr("info: "+identify(info));
    trr("client: "+identify(client));
    trr("s1: "+s1);
    trr("s2: "+s2);
    trr("s3: "+s3);
    trr("s4: "+s4);
    trr("s5: "+s5);
    if(scan < 3 || (!mudinfo[s1] && scan != 5)){
        trr("\nwrong size packet\n");
        return;
    }
    if(s3){
        sscanf(s3,"version=%f",vers);
    } 
    if(undefinedp(vers)) vers = 2.0;
    trr("vers: "+identify(vers));
    if(!s5 && mudinfo[s1]) s5 = mudinfo[s1]["serverpass"];
    //if(grepp(s5,"\n")) tc("OH YEAH");
    //else tc("bummer.");
    s5 = replace_string(s5,"\n","");
    s5 = replace_string(s5," SHA256","");
    if(!mudinfo[s1]){
        newmud = 1;
        trr("NEW MUD "+s1);
        mudinfo[s1] = ([ "clientpass" : s2, "serverpass" : s5, 
                "password" : random_numbers(9,1) ]);
    }
    else {
        if(mudinfo[s1]["clientpass"] != s2){ 
            trr("BAD PASSWORD! Wanted "+mudinfo[s1]["clientpass"]+
                    ", got: "+s2,"red");
            mudinfo[s1]["password"] = random_numbers(10,1);
        }
        else trr("Good password for "+s1);
        trr(s1 + " I3 password: "+mudinfo[s1]["password"]);
    }
    passwd = mudinfo[s1]["password"];
    if(mudinfo[s1]["port"]) pport = mudinfo[s1]["port"];
    foreach(mixed key, mixed val in mudinfo[s1]){
        if(member_array(key, nix) == -1){
            other[key] = val;
        }
    }
    other["imc_version"] = vers;
    packet = ({ "startup-req-3", 5, s1, 0, router, 0, passwd, 1, 1,
            pport, 0, 0, " ", " ", " ", " ", " ", " ",
            (["channel" : 1, "who" : 1, "tell" : 1]), other });
    trr("%^WHITE%^constructed packet: "+identify(packet), "black");
    ROUTER_D->update_imc2(s1, other);
    this_object()->acknowledge_startup(fd, s1, vers);
#if CALL_OUT_COMPLETE_STARTUP
    call_out("complete_startup", 0, fd, packet, s1, other);
#else
    ROUTER_D->read_callback(fd, packet);
    ROUTER_D->broadcast_mudlist(s1);
    ROUTER_D->broadcast_chanlist("foo",0);
    mudinfo[s1]["password"] = ROUTER_D->query_mudinfo()[s1]["password"];
    trr(s1 + " i3 password: "+mudinfo[s1]["password"]);
#endif
}

#if CALL_OUT_COMPLETE_STARTUP
static complete_startup(int fd, mixed packet, string s1, mixed other){
    string checkmud = ROUTER_D->query_connected_fds()[fd]; 
    if(checkmud){
        return;
    }
    ROUTER_D->read_callback(fd, packet);
    ROUTER_D->update_imc2(s1, other);
    ROUTER_D->broadcast_mudlist(s1);
    mudinfo[s1]["password"] = ROUTER_D->query_mudinfo()[s1]["password"];
    trr(s1 + " i3 password: "+mudinfo[s1]["password"]);
}
#endif

varargs void acknowledge_startup(int fd, string mud, mixed vers){
    string syn, ack, passwd, ver;
    string server = ROUTER_D->GetRouterName();
    ver = vers+"";
    trr("ack for "+mud+" ver is: "+ver);
    if(mudinfo && mudinfo[mud] && mudinfo[mud]["serverpass"])
        passwd = mudinfo[mud]["serverpass"];
    ack = "PW "+server+" "+passwd+" version="+ver+" LPMuds.net";
    syn = replace_string(ack, "\n","#");
    syn = replace_string(syn, "\r","^");
    ack = replace_string(ack, "\n","");
    trr(mud+" ("+fd+") ack: "+ack);
    write_data(fd, ({ack}), 1, vers);
}

void read_callback(mixed fd, mixed info){
    mixed womble = info;
    mixed packet = packetize(info);
    mixed ret;
    mapping datamap = string_to_mapping(info);
#if 1
    womble = replace_string(womble, "\n", "(LF)");
    womble = replace_string(womble, "\r", "(CR)");
    //trr("packet: "+identify(womble),"yellow");
    womble = packetize(womble);
    //trr("packet: "+identify(womble),"yellow");
#endif
    //trr("whee! read_callback("+fd+", "+identify(womble)+")","cyan");
    if(stringp(info) && last(info,1) == "\n") info = truncate(info,1);
    //trr("Mapping form: "+identify(datamap)+")","cyan");
    if(stringp(info) && !strsrch(info,"PW ")){
        mixed start;
        info = replace_string(info,"\r","\n");
        start = explode(info, "\n");
        if(sizeof(start)) info = start[0];
        construct_startup(fd, info);
        return;
    }
    if(!packet || !mudinfo[packet[2]] || 
            ROUTER_D->query_connected_fds()[fd] != packet[2]){
        write_data(fd, "Your connection isn't registered as " +
                (packet ? packet[2] : "anything at all"));
        trr("IMC2: BAD CONNECTION ON "+fd, "red");
        close_connection(fd);
        return;
    }
    if(packet[0] == "ice-msg-b"){
        string tmp;
        if(!stringp(datamap["text"])) datamap["text"] = itoa(datamap["text"]);
        sscanf(packet[6],"%*shannel=%*s:%s %*s",tmp);
        if(datamap["emote"]){
            ret = ({ "channel-e" });
            if(datamap["emote"] == 2){
                string tmp2 = datamap["text"];
                tmp2 = replace_string(tmp2, packet[3]+"@"+packet[2], "$N");
                datamap["text"] = tmp2;
            }
            else {
                string tmp2 = "$N " + datamap["text"];
                datamap["text"] = tmp2;
            }
        }
        else ret = ({ "channel-m" });
        ret += packet[1..5];
        ret += ({ tmp, capitalize(packet[3]), datamap["text"] });
        //tc("ret: "+identify(ret));
        ROUTER_D->read_callback(fd, ret);
        return;
    }
    if(packet[0] == "is-alive" || packet[0] == "keepalive-request"){
        int update;
        mixed data;
        //trr(packet[0],"white");
        if(sizeof(packet) < 6){
            //trr("Wrong packet size.","white");
            write_data(fd, "Wrong packet size.");
            return;
        }
        data = parse_info(packet[6]);
        //trr("data: "+identify(data));
        foreach(mixed key, mixed val in data){
            //trr("key: "+identify(key)+", val: "+identify(val),"green");
            if(!mudinfo[packet[2]][key] ||
                    mudinfo[packet[2]][key] != val){
                //trr("key: "+identify(key)+", val: "+identify(val),"red");
                update = 1;
                mudinfo[packet[2]][key] = val;
            }
        }
        if(update){
            //trr("trying to update","white");
            ROUTER_D->update_imc2(packet[2], data);
        }
    }
    if(packet[0] == "keepalive-request"){
        //trr("trying to send mudlist");
        ROUTER_D->send_full_mudlist(packet[2]);
        return;
    }
    if(packet[0] == "ice-refresh"){
        //trr("trying to send chanlist");
        ROUTER_D->broadcast_chanlist("foo",packet[2]);
        return;
    }
    if(packet[0] == "tell"){
        string msg;
        //trr("trying to send tell");
        msg = replace_string(packet[6],"text=","");
        ret = ({ "tell", 5, packet[2], packet[3], packet[4], 
                packet[5], packet[3], msg });
        ROUTER_D->read_callback(fd, ret);
        return;
    }
    if(packet[0] == "who-reply"){
        string *tmp_ret = ({});
        string tmp = packet[6];
        tmp = replace_string(tmp,"text=","");
        if(is_lpmud(packet[4])){
            tmp = replace_string(tmp,"\\n","\n");
        }
        foreach(string element in explode(tmp,"\n")){
            tmp_ret += ({ ({ element, 0, "" }) });
        }
        ret = ({ "who-reply", 5, packet[2], 0, packet[4],
                packet[5], tmp_ret });
        ROUTER_D->read_callback(fd, ret);
    }
    if(packet[0] == "who"){
        ret = ({ "who-req", 5, packet[2], packet[3], packet[4], 0 });
        ROUTER_D->read_callback(fd, ret);
    }
}

void broadcast_data(mapping targets, mixed data){
    //ssocket = find_object(SSOCKET_D);
    //tc("targets: "+identify(targets));
    validate();
    //tc("data: "+identify(data));
    foreach(int *arr in unique_array(values(targets), (: $1 :))){
        //tc("arr[0]: "+identify(arr[0]));
        //if(!ssocket) tc("FUCK");
        //else 
        write_data(arr[0], data);
    }
    //if(ssocket) ssocket->broadcast_data(targets, data);
    //tc("C");
}
