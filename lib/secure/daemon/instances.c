#include <lib.h>
#include <news.h>
#include <save.h>
#include <daemons.h>
#include <commands.h>
#include <message_class.h>
#include <talk_type.h>
#include <secrets.h>
#include <socket.h>
#include NETWORK_H

inherit LIB_DAEMON;

mapping InstData = ([]);
mapping sockets = ([]);
static string SaveFile;
string Name, Myname;
static int verbose = 1;
static private int icp_socket;

varargs static void yenta(string arg, string clr){
    if(verbose){
        //tc(arg, (clr || "magenta"));
    }
    log_file("/secure/log/icp", timestamp()+" "+strip_colours(arg)+"\n");
}

varargs static void validate(int i){
    //tc("validate "+mud_name()+" socket_status("+i+"): "+identify(socket_status(i)), "blue");
    if(!undefinedp(i)){
        //tc("socket_status("+i+"): "+identify(socket_status(i)), "blue");
        if(!socket_status(i) || !socket_status(i)[5]){
            yenta(mud_name()+" stack: "+get_stack());
            yenta("%B_BLACK%^BAD SOCKET ALERT. fd "+i+":  "+
                    identify(socket_status(i))+"\n","red");
            error("Bad socket, fd "+i);
        }
    }
    if(previous_object() != this_object() && !((int)master()->valid_apply(({ "ASSIST" }))) ){
        yenta("SECURITY ALERT: validation failure in INSTANCES_D.\n","red");
        error("Illegal attempt to access router socket daemon: "+get_stack()+
                " "+identify(previous_object(-1)));
    }
}

static void create() {
    daemon::create();
    Myname = "global";
    if(!sizeof(InstData)) InstData = ([]);
    if(!sizeof(sockets)) sockets = ([]);
    SaveFile = save_file(SAVE_INSTANCES);
    SetSaveFile(SaveFile);
    if(!file_exists(SaveFile) && file_exists(old_savename(SaveFile))){
        cp(old_savename(SaveFile), SaveFile);
    }
    SetNoClean(1);
    if(file_exists(SaveFile)){
        //tc("SaveFile: "+SaveFile);
        unguarded((: RestoreObject(SaveFile) :));
    }
    //else tc("hmm.");
    if(ENABLE_INSTANCES){
    }
    call_out("Setup", 0);
    set_heart_beat(10);
}

string GetMyInstanceName(){
    return Myname;
}

mixed InstCreate(string name, string addy, int port){
    object prev = previous_object();
    string newglobal, newconf, pfile = base_name(this_player());
    if(base_name(prev) != CMD_INSTCONFIG) return 0;
    if(ENABLE_INSTANCES){
        return "Only the global instance can create instances.";
    }
    if(InstData[name]) return name + " already exists.";
    foreach(mixed key, mixed val in InstData){
        if(val && val["port"] == port){
            return key + " is already on port "+port;
        }
    }
    InstData[name] = ([ "addy" : addy, "port" : port ]);
    unguarded( (: SaveObject, SaveFile, 1 :) );
    cp(NETWORK_H, "/secure/include/network."+port+".h");
    cp(ROOMS_H, "/secure/include/rooms."+port+".h");
    cp(SECRETS_H, "/secure/include/secrets."+port+".h");
    cp(SAVE_IMC2+".o", SAVE_IMC2 + "." + port + ".o");
    cp(SAVE_MAP+".o", SAVE_MAP + "." + port + ".o");
    cp(SAVE_ROOMS+".o", SAVE_ROOMS + "." + port + ".o");
    cp(SAVE_RACES+".o", SAVE_RACES + "." + port + ".o");
    cp(SAVE_CLASSES+".o", SAVE_CLASSES + "." + port + ".o");
    cp(NEWS_WELCOME, NEWS_WELCOME + "." +port);
    //tc("pfile: "+pfile);
    cp(pfile+".o", pfile+"."+port+".o");
    newglobal = read_file("/secure/cfg/global_template.cfg");
    if(newglobal){
        newglobal = replace_string(newglobal, "TEMPLATE_CONFIG",
                "/secure/include/config."+port+".h");
        newglobal = replace_string(newglobal, "TEMPLATE_NETWORK",
                "/secure/include/network."+port+".h");
        newglobal = replace_string(newglobal, "TEMPLATE_ROOMS",
                "/secure/include/rooms."+port+".h");
        newglobal = replace_string(newglobal, "TEMPLATE_SECRETS",
                "/secure/include/secrets."+port+".h");
    }
    write_file("/secure/include/global."+port+".h", newglobal, 1);
    newconf = read_file(CONFIG_H);
    if(newconf){
        string *confarr = explode(newconf, "\n");
        newconf = "";
        foreach(string line in confarr){
            if(!strsrch(line, "#define DISABLE_INTERMUD")){
                line = "#define DISABLE_INTERMUD         1";
            }
            if(!strsrch(line, "#define DISABLE_IMC2")){
                line = "#define DISABLE_IMC2             1";
            }
            if(!strsrch(line, "#define ENABLE_INSTANCES")){
                line = "#define ENABLE_INSTANCES         1";
            }
            if(!strsrch(line, "#define LOG_REMOTE_CHANS")){
                line = "#define LOG_REMOTE_CHANS         0";
            }
            if(!strsrch(line, "#define LOG_LOCAL_CHANS")){
                line = "#define LOG_LOCAL_CHANS          0";
            }
            newconf +=  line + "\n";
        }
        write_file("/secure/include/config."+port+".h", newconf, 1);
    }
    if(!query_windows()){
        string newcfg = read_file("/secure/cfg/instance_template.cfg");
        string oldcfg = read_file("/secure/cfg/mudos.cfg");
        if(newcfg && oldcfg){
            string mlib, mbin;
            sscanf(oldcfg, "%*smudlib directory :%s\n", mlib);
            sscanf(oldcfg, "%*sbinary directory :%s\n", mbin);
            newcfg = replace_string(newcfg, "TEMPLATE_NAME", name);
            newcfg = replace_string(newcfg, "TEMPLATE_PORT", itoa(port));
            if(mlib) mlib = trim(mlib);
            if(mbin) mbin = trim(mbin);
            newcfg = replace_string(newcfg, "TEMPLATE_LIB", mlib);
            newcfg = replace_string(newcfg, "TEMPLATE_BIN", mbin);
            newcfg = replace_string(newcfg, "TEMPLATE_GLOBAL", 
                    "global."+port+".h");
            write_file("/secure/cfg/mudos."+port+".cfg", newcfg, 1);
        }
    }
    unguarded( (: SaveObject, SaveFile, 1 :) );
    return 1;
}

mixed InstDelete(mixed arg){
    object prev = previous_object();
    mixed which;
    if(base_name(prev) != CMD_INSTCONFIG) return 0;
    if(ENABLE_INSTANCES){
        return "Only the global instance can delete instances.";
    }
    if(InstData[arg]) which = arg;
    else {
        if(stringp(arg)) which = atoi(arg);
        else if(intp(arg)) which = arg;
        foreach(mixed key, mixed val in InstData){
            if(val && val["port"] == which){
                which = key;
                break;
            }
        }
    }

    if(which && InstData[which]){
        mixed port = itoa(InstData[which]["port"]);
        rm("/secure/include/config."+port+".h");
        rm("/secure/include/global."+port+".h");
        rm("/secure/include/rooms."+port+".h");
        rm("/secure/include/network."+port+".h");
        rm("/secure/include/secrets."+port+".h");
        rm("/secure/cfg/mudos."+port+".cfg");
        rm(NEWS_WELCOME + "." + port);
        this_object()->ProcessClose(which);
        InstData[which] = 0;
        unguarded( (: SaveObject, SaveFile, 1 :) );
        return 1;
    }
    else return "No such instance exists.";
}

string array GetInstances(){
    return filter(keys(InstData), (: $1 :));
}

mapping GetInstData(){
    mapping ret = ([]);
    foreach(mixed key, mixed val in InstData){
        if(key) ret[key] = copy(val);
    }
    return ret;
}

static void SendData(mixed fd, mixed data){
    int array targets = ({});
    //tc("SendData "+fd+", "+identify(data));
    if(stringp(fd) && !undefinedp(InstData[fd])) fd = InstData[fd]["fd"];
    if(fd < 0){
        foreach(string name in keys(InstData)){
            if(name && InstData[name]) targets += ({ InstData[name]["fd"] });
        }
    }
    else targets = ({ fd });
    foreach(int target in targets){
        //tc("target: "+target, "blue");
        this_object()->write_data(target, data);
    }
}

varargs void SendWhoUpdate(string name, int status){
    mapping data = ([ "status" : status ]);
    object ob = find_player(name);
    if(ob && !ob->GetInvis()){
        string title = ob->GetShort();
        if(sizeof(strip_colours(title)) > 50){
            title = capitalize(name)+" the Long-Titled.";
        }
        data["title"] = title;
        data["level"] = ob->GetLevel();
        data["arch"] = archp(ob);
        data["creator"] = creatorp(ob);
        data["builder"] = builderp(ob);
        if(interactive(ob)) data["status"] = 1;
        else data["status"] = -1;
    }
    else data["status"] = 0;
    //tc("SendWhoUpdate("+name+", "+identify(data)+")","green");
    SendData(-1, ({ "who-update", 5, Myname, name, 0, 0, data }) );
} 

void UpdateInvis(int i){
    object ob = previous_object();
    string name;
    //tc("UpdateInvis: "+identify(ob));
    if(!ob || !interactive(ob)) return;
    name = ob->GetKeyName(); 
    if(!sizeof(name)) return;
    //tc("UpdateInvis: "+identify(name));
    call_out("SendWhoUpdate", 0, name);
}

void SendTell(string who, string msg){
    object ob = this_player();
    string sender, vname;
    if(!ob || ob->GetForced()) return;
    sender = ob->GetKeyName();
    vname = ob->GetName();
    SendData(-1, ({ "tell", 5, Myname, sender, 0, who, vname, msg }) );
}

static void ProcessStartup(mixed data, string addy, int port, int fd){
    string name = data[2];
    InstData[name] = ([]);
    InstData[name]["addy"] = addy;
    InstData[name]["port"] = port;
    InstData[name]["fd"] = fd;
    InstData[name]["online"] = 1;
    InstData[name]["users"] = ([]);
    if(sizeof(data[7])){
        foreach(mixed foo in data[7]){
            if(stringp(foo)) InstData[name]["users"][foo] = 1;
        }
    }
    if(ENABLE_INSTANCES){
        Myname = data[4];
        //tc("startup from global processed for "+name+": "+identify(InstData[name]),"white");
        call_out("SendStartup", 1, fd);
    }
    else {
        //tc("startup from non-global processed for "+name+": "+identify(InstData[name]),"black");
    } 
}

static void ProcessClose(string name){
    if(!sizeof(InstData[name])) return;
    this_object()->close_connection(InstData[name]["fd"]);
    InstData[name]["fd"] = -1;
    InstData[name]["online"] = 0;
}

static void ProcessWhoUpdate(mixed data){
    //tc("ProcessWhoUpdate("+identify(data)+")", "blue");
    if(!sizeof(InstData[data[2]])) return;
    if(!sizeof(InstData[data[2]]["users"])){
        InstData[data[2]]["users"] = ([]);
    }
    InstData[data[2]]["users"][data[3]] = data[6];
}

static void ProcessTell(mixed data){
    object who = find_player(data[5]);
    string ret;
    if(!who) return;
    ret = "%^BOLD%^RED%^"+capitalize(data[3]) +
        " tells you:%^RESET%^ " + data[7];
    who->eventPrint(ret, MSG_CONV);
    who->SetProperty("reply", data[3]);
    who->SetProperty("reply_time", time());
    who->eventTellHist(ret);
}

static void ProcessShout(mixed data){
    Name = capitalize(data[3]);
    users()->eventHearTalk(this_object(), 0, TALK_WORLD,
            "shout", data[6][0], data[6][1]);
}

string GetName(){
    return Name;
}

static void ReceiveICPData(mixed data, string addy, int port, int fd){
    string name;
    //tc("%^B_BLACK%^ReceiveICPData: "+identify(data),"cyan");
    if(!arrayp(data)) return;
    //tc("-2");
    if(sizeof(data) < 7) return;
    //tc("-1");
    name = data[2];
    //tc("name: "+name);
    if(data[0] != "startup-req"){
        //tc("a");
        if( InstData[name] && InstData[name]["online"]){
            //tc("b");
            if(InstData[name]["addy"] != addy) return;
            //tc("c");
            if(InstData[name]["port"] != port ) return;
            //tc("d");
            if(!undefinedp(InstData[name]["fd"]) && 
                    InstData[name]["fd"] != fd) return;
            //tc("e");
        }
        else{
            //tc("f");
            return;
        }
        //tc("g");
    }
    //tc("win so far", "red");
    switch(data[0]){
        case "startup-req" :
            //tc("STARTUP YALL: "+identify(data),"green");
            if(data[6] != INSTANCE_PW) return;
            else ProcessStartup(data, addy, port, fd);
        break;
        case "channel-p" :
            //tc("chan!");
            CHAT_D->eventSendChannel(data[6]...);
        break;
        case "who-update":
            //tc("who!");
            ProcessWhoUpdate(data);
        break;
        case "tell":
            ProcessTell(data);
        break;
        case "shout":
            ProcessShout(data);
        break;
        case "close-req" :
            ProcessClose(name);
        break;

        default :
        //tc("i dono");
        break;
    }
    if(undefinedp(InstData[name]["fd"])) InstData[name]["fd"] = fd;
}

string *GetRemoteUsers(string inst){
    string *ret = ({});
    if(!InstData || !InstData[inst] || !InstData[inst]["users"]) return ret;
    else ret = filter(keys(InstData[inst]["users"]),
            (: mapp(InstData[$(inst)]["users"][$1])
             && InstData[$(inst)]["users"][$1]["status"] > 0 :) );
    return ret;
}

void eventSendChannel(string name, string ch, string msg, int emote, 
        string target, string targmsg){
    mixed packet = ({ name, ch, msg, emote, target, targmsg });
    if(base_name(previous_object()) != CHAT_D) return;
    //tc("name: "+name);
    //tc("ch: "+ch);
    //tc("msg: "+msg);
    //tc("emote: "+emote);
    //tc("target: "+target);
    //tc("targmsg: "+targmsg);
    SendData(-1, ({ "channel-p", 5, Myname, 0, 0, 0, packet }) );
}

void eventSendShout(string msg, string lang){
    string name;
    mixed packet = ({ msg, lang });
    object ob = previous_object();
    if(!ob || !interactive(ob) || ob->GetForced()) return;
    name = ob->GetName();
    SendData(-1, ({ "shout", 5, Myname, name, 0, 0, packet }) );
}

static void eventWrite(mixed *packet){
    SendData(packet[4], packet);
}

static void close_connection(int fd){
    int sockerr;
    mixed *sockstat = ({});
    //tc("close_connection("+fd+")");
    validate();
    sockstat = socket_status(fd);
    if(!sockstat || !sizeof(sockstat)) return;
    if(sockstat[1] == "LISTEN") return;
    yenta("About to try closing socket: "+fd, "white");
    yenta("Pre-closing state: "+sockstat[1],"yellow");
    sockerr = socket_close(fd);
    yenta("closing socket:"+fd,"white");
    yenta("closing sockerr:"+sockerr,"white");
    yenta("Post-closing state: "+socket_status(fd)[1],"yellow");
    yenta("---\n","white");
}

static void close_callback(int fd){
    //tc("close_callback("+fd+")");
    //validate(fd);
    yenta("close_callback: fd="+fd+"\n");
    if(socket_status(fd)[1] == "LISTEN") return;
    if(socket_status(fd)[1] == "DATA_XFER") return;
    close_connection(fd);
}

static void listen_callback(int fd){
    mixed fdstat,newfd;
    //tc("LISTEN CALLBACK("+fd+")");
    validate();

    if ((newfd = socket_accept(fd, "read_callback", "write_callback")) < 0) {
        //tc("listen_callback couldn't accept socket "+fd+", errorcode "+newfd);
        return;
    }
    else {
        yenta("socket_accepted: "+newfd+
                ", "+identify(socket_status(newfd))+"\n","white");
    }
}

static void read_callback(int fd, mixed info){
    mixed sstat;
    string addy;
    int port;
    //tc("read_callback("+fd+")");
    //tc("%^B_BLUE%^read_callback for fd "+fd+", info: "+identify(info),"yellow");
    validate(fd);
    //tc("%^B_BLUE%^passed validation", "red");
    sstat = socket_status(fd);
    if(sizeof(sstat) > 3){
        string a, b, c, d;
        int p, i;
        i = sscanf(sstat[4],"%s.%s.%s.%s.%d",a,b,c,d,p);
        if(i > 4){
            port = (p - OFFSET_ICP); 
            addy = a+"."+b+"."+c+"."+d;
        }
    }
    //tc("1");
    if(bufferp(info)){
        yenta("fd "+fd+" is sending me buffer data!");
        yenta("As far as I can tell, it is: "+identify(read_buffer(info)),"blue");
    }
    else yenta("%^WHITE%^data from fd "+fd+":\n%^BLUE%^"+identify(info));
    //tc("2");
    ReceiveICPData(info, addy, port, fd);
}

static void write_callback(int fd){
    //tc("%^B_GREEN%^write_callback("+fd+")","yellow");
    validate(fd);
    if(!sockets[fd]) return;
    if(sockets[fd]["write_status"] == EEALREADY) {
        this_object()->write_data(fd, sockets[fd]["pending"]);
        sockets[fd]["pending"] = 0;
    } 
    else {
        sockets[fd]["write_status"] = EESUCCESS;
    }
}

static void write_data_retry(int fd, mixed data, int counter){
    int rc;
    int maxtry = 20;
    //tc("write_data_retry");
    validate(fd);
    if (counter == maxtry) {
        //tc("Could not write data.");
        return;
    }
    rc = socket_write(fd, data);
    if(!sockets) sockets = ([]);
    if(!sockets[fd]){
        sockets[fd]=([]);
    }
    sockets[fd]["write_status"] = rc;
    //tc("fd"+fd+" socket_error("+rc+"): "+socket_error(rc));
    switch (rc) {
        case EESUCCESS:
            break;
        case EEALREADY:
            sockets[fd]["pending"] = data;
            break;
        case EECALLBACK:
            break;
        case EESECURITY:
            break;
        case EEFDRANGE:
            break;
        case EENOTCONN:
            break;
        case EEBADF:
            break;
        default:
            if (counter < maxtry) {
                if(counter < 2 || counter > maxtry-1)
                    //tc("ICP_D write_data_retry "+counter+" to "+
                    //INSTANCES_D->query_connected_fds()[fd]+", fd"+fd+" error,  code "+rc+": " + socket_error(rc));
                    call_out( (: write_data_retry :), 2 , fd, data, counter + 1 ); 
                return;
            }
    }
}

static int write_data(int fd, mixed data){
    int ret;
    //tc("write_data "+fd,"black");
    validate(fd);
    //tc("write_data: "+fd,"white");
    write_data_retry(fd, data, 0);
    //ret = socket_write(fd, data);
    //tc("write_data: "+ret);
    return ret;
}

static void Setup(){
    yenta("icp setup got called");
    if ((icp_socket = socket_create(MUD, "read_callback", "close_callback")) < 0){
        log_file("/secure/log/icp","setup: Failed to create socket.\n");
        //tc("Failed to create socket");
        return;
    }
    if (socket_bind(icp_socket, PORT_ICP) < 0) {
        socket_close(icp_socket);
        log_file("/secure/log/icp","setup: Failed to bind socket to port.\n");
        //tc("Failed to bind socket to port");
        return;
    }
    if (socket_listen(icp_socket, "listen_callback") < 0) {
        socket_close(icp_socket);
        log_file("/secure/log/icp","setup: Failed to listen to socket.\n");
        //tc("Failed to listen to socket");
        return;
    }
    log_file("/secure/log/icp","icp setup ended\n");
    //tc("icp setup success on "+icp_socket);
}

int eventCreateSocket(string host, int port){
    int x, ret;

    x = socket_create(MUD, "read_callback", "close_callback");
    if( x < 0 ) {
        yenta("Error in socket_create(): "+socket_error(x));
        return 0;
    }
    //tc("x1: "+x);
    ret = x;
    x = socket_bind(x, 0);
    //tc("x2: "+x);
    if( x != EESUCCESS ) {
        socket_close(ret);
        log_file("/secure/log/icp", "Error in socket_bind(): "+x);
        return 0;
    }
    x = socket_connect(ret, host + " " + (port + OFFSET_ICP), 
            "read_callback", "close_callback");
    //tc("x3: "+x);
    if( x != EESUCCESS ) {
        socket_close(ret);
        yenta("Error in socket_connect(): "+socket_error(x));
        return x;
    }
    //tc("x4: yay!");
    return ret;
}

int DoConnect(string ip, int port, string myname, string name){
    int sstat;
    validate();
    sstat = eventCreateSocket(ip, port);
    //tc("sstat: "+sstat);
    if( sstat < 0 ) return;
    if(!sockets) sockets = ([]);
    sockets[sstat] = ([]);
    return sstat;
}

static void InstConnect(string wat){
    int ret = DoConnect(InstData[wat]["addy"],
            InstData[wat]["port"],"global",wat);
    //tc("ret: "+ret);
    if(ret > -1){
        InstData[wat]["fd"] = ret;
        call_out("SendStartup", 1, ret);
        //this_object()->SendStartup(ret);
    }
    else{
        //tc("InstConnect FAIL", "red");
        yenta("InstConnect FAIL", "red");
    }
}

static void SendStartup(int fd){
    string name;
    //tc("SendStartup("+fd+")");
    foreach(mixed key, mixed val in InstData){
        if(key && val["fd"] == fd){
            name = key;
            break;
        }
    }
    //tc(ctime(time())+" sending startup to: "+name,"white");
    SendData(fd, 
            ({"startup-req", 5, Myname, 0, name, 0, INSTANCE_PW, local_users()}));
    foreach(string user in local_users()){
        call_out("SendWhoUpdate", 0, user, 1);
    }
}

static void CheckConnections(){
    mixed socks = socket_status();
    mapping conns = ([]);
    if(ENABLE_INSTANCES){
        return;
    }
    if(!sizeof(keys(InstData))){
        return;
    }
    socks = filter(socks, (: $1[5] == this_object() :) );
    foreach(mixed arr in socks){
        if(sizeof(arr) > 4){
            string a, b, c, d;
            int p, i;
            i = sscanf(arr[4],"%s.%s.%s.%s.%d",a,b,c,d,p);
            if(i < 5) continue;
            foreach(mixed key, mixed val in InstData){
                if(key && mapp(val)){
                    //tc(key+"[\"port\"]: "+val["port"]);
                    //tc("p: "+p);
                    if(val["port"] == (p - OFFSET_ICP) &&
                            val["addy"] == a+"."+b+"."+c+"."+d){
                        conns[key] = val;
                    }
                }
            }
        }
    }
    foreach(string foo in keys(InstData)){
        if(!foo || !InstData[foo]) continue;
        InstData[foo]["online"] = 0;
        if(member_array(foo, keys(conns)) == -1){
            InstConnect(foo);
        }
        else InstData[foo]["online"] = 1;
    }
}

void heart_beat(){
    if(!ENABLE_INSTANCES) CheckConnections();
}
