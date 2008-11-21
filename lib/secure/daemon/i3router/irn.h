#include <network.h>
#include <save.h>
#include <daemons.h>

static string my_name = ROUTER_NAME;
static string my_password = IRN_PASSWORD;
static string *ok_ips = ({});
static int irn_reconnect = 0;
static int irn_timeout = 120;
static int irn_maxtry = 32;
static int convert_channel = 1;
static int convert_channel2 = 0;
mapping PingMap = ([]);

#ifndef PRODUCTION_ROUTER
static int irn_enabled = 0;
static int irn_ping_enabled = 0;
static mapping routers = ([
  "*i6" : ([ "ip" : "149.152.218.102", "port" : 25, "password" : IRN_PASSWORD1 ]),
  "*i5" : ([ "ip" : "204.209.44.3", "port" : 8180, "password" : IRN_PASSWORD2 ]),
  //"*gimel" : ([ "ip" : "192.168.0.224", "port" : 4301, "password" : IRN_PASSWORD4 ])
]);
#else
static int irn_enabled = 1;
static int irn_ping_enabled = 1;
static mapping routers = ([
  "*wpr" : ([ "ip" : "195.242.99.94", "port" : 8080, "password" : IRN_PASSWORD4 ]),
  "*i4" : ([ "ip" : "204.209.44.3", "port" : 8080, "password" : IRN_PASSWORD4 ]),
  "*yatmim" : ([ "ip" : "149.152.218.102", "port" : 23, "password" : IRN_PASSWORD2 ]),
  "*dalet" : ([ "ip" : "204.209.44.3", "port" : 8787, "password" : IRN_PASSWORD3 ])
]);
#endif

static mapping chan_conv = ([
  //"*adsr" : ([ "imud_gossip" : "free_speech" ]),
  //"*i4" : ([ "free_speech" : "imud_gossip", "imud_gossip" : "free_speech" ]),
  //"*yatmim" : ([ "free_speech" : "imud_gossip", "imud_gossip" : "free_speech" ]),
  //"*krakatoa" : ([ "free_speech" : "imud_gossip", "imud_gossip" : "free_speech" ]),
  //"*stroggili" : ([ "imud_gossip" : "free_speech" ])
]);

mapping irn_connections = ([]);
mapping irn_sockets = ([]);

static void write_data(int fd, mixed data);
static varargs void SendList(mixed data, int fd, string type);
varargs void SendWholeList(int fd,string type);
static void begin_socket_handoff(int i);

void build_ok_ips(){
    ok_ips = ({});
    if(irn_enabled){
        foreach(mixed key, mixed val in routers){
            if(key == my_name) continue;
            ok_ips += ({ routers[key]["ip"] });
        }
        ok_ips = singular_array(ok_ips);
    }
}

void irn_checkstat(){
    int setuptype = 0;
    mixed sstat;
    if(!irn_enabled || !sizeof(routers)) return;

    build_ok_ips();

    foreach(mixed key, mixed val in routers){
        if(key != my_name && (!irn_connections[key] || irn_connections[key]["fd"] == -1)){
            trr("irn_checkstat: hmmm no irn_connections["+key+"]? see: "+identify(irn_connections));
            foreach(mixed key2, mixed val2 in mudinfo){
                if(val2["router"] == key && !mudinfo[key2]["disconnect_time"]){
                    trr(key+" is down, removing its mud: "+key2);
                    this_object()->disconnect_mud(key2, 1);
                }
            }
        }
    }

    foreach(mixed key, mixed val in irn_sockets){
        sstat = socket_status(key);
        if(!sstat || sstat[1] != "DATA_XFER"){
            trr("IRN checkstat: removing socket record for: "+identify(key));
            map_delete(irn_sockets, key);
        }

        if(!irn_connections[val["name"]] || 
          irn_connections[val["name"]]["fd"] != key){
            trr("IRN checkstat: there is a conflicted record in irn_sockets.");
            //map_delete(irn_sockets, key);
        }
    }

    foreach(mixed key, mixed val in irn_connections){
        if(!PingMap) PingMap = ([]);
        if(!key ||!sizeof(key)|| !val) continue;

        if(PingMap[key] && !this_player()){
            //trr("IRN pingmap for "+key+" is "+ctime(PingMap[key]));
        }

        if(irn_ping_enabled && key != my_name && 
          (time() - PingMap[key]) > irn_timeout ){
            trr("IRN ping timeout for "+key+"!","red");
            trr("Last ping was "+time_elapsed(time() - PingMap[key])+" ago.");
            if(irn_connections[key] && irn_connections[key]["fd"]){
                map_delete(irn_sockets, irn_connections[key]["fd"]);
                map_delete(irn_connections, key);
                this_object()->irn_setup(0, key);
                return;
            }
        }

        if(!irn_sockets[irn_connections[key]["fd"]] ||
          irn_sockets[irn_connections[key]["fd"]]["name"] != key ||
          (sstat && sstat[1] != "DATA_XFER")){
            trr("IRN checkstat: removing socket and connection record for: "+identify(key));
            map_delete(irn_sockets, irn_connections[key]["fd"]);
            map_delete(irn_connections,key);
            return;
        }
    }

    //tc("Looking to disconnect dead irn sockets.");
    foreach(mixed key, mixed val in irn_sockets){
        int restart;

        sstat = socket_status(key);
        if(!sstat || sstat[1] != "DATA_XFER"){
            trr("IRN checkstat: removing socket record for: "+identify(key));
            map_delete(irn_sockets, key);
            restart = 1;
        }
        if(sizeof(irn_connections) && irn_connections[irn_sockets[key]["name"]] && irn_connections[irn_sockets[key]["name"]]["fd"] != key){
            trr("IRN checkstat: removing conflicted connection and socket "+key);
            map_delete(irn_sockets, irn_connections[irn_sockets[key]["name"]]["fd"]);
            map_delete(irn_connections, irn_sockets[key]["name"]);
            map_delete(irn_sockets, key);
            restart = 1;
        }

        if(restart){
            trr("%^B_BLUE%^I be wantin to restart "+val["name"]+".\nrouters: "+identify(routers));
            if(sizeof(routers) > 2) this_object()->irn_setup(0, val["name"]);
            return;
        }
        else setuptype = 1;
    }

    if(!sizeof(irn_connections) || !sizeof(irn_sockets)){
        trr("IRN scheduling an IRN socket clear","red");
        setuptype = 1;
    }

    if((sizeof(routers) -1) != sizeof(irn_connections)){
        string *stragglers = ({});
        trr("expected "+(sizeof(routers) -1)+" connections, "+identify(routers));
        trr("got: "+sizeof(irn_connections)+", "+identify(irn_connections));
        foreach(string key, mixed val in routers){
            if(key != my_name && 
              member_array(key, keys(irn_connections)) == -1){
                stragglers += ({ key });
            }
        }
        trr("stragglers: "+identify(stragglers));
        foreach(string key in stragglers){
            trr("reload irn for "+key);
            this_object()->irn_setup(0, key);
        }
    }

}

static int GoodPeer(int fd, mixed data){
    string ip = explode(socket_address(fd)," ")[0];
    if(!irn_enabled) return 0;
    //trr("IRN: hit GoodPeer","yellow");
    if(member_array(ip,ok_ips) == -1){
        server_log("irn: bad ip: "+ip);
        server_log("ok_ips: "+identify(ok_ips));
        return 0;
    }

    if(!data || !arrayp(data) || member_array(data[2], keys(routers)) == -1){
        trr("IRN: unknown peer");
        return 0;
    }
    if(data[4] != my_name){
        trr("IRN: they don't know my name. I am "+my_name+". They think I am: "+identify(data[4]));
        return 0;
    }

    if(data[6]["client_password"] != routers[data[2]]["password"]){
        trr("IRN: wrong client password");
        return 0;
    }
    if(data[6]["server_password"] != my_password){
        trr("IRN: wrong server password");
        return 0;
    }
    if(!irn_connections[data[2]]) irn_connections[data[2]] = ([]);
    irn_connections[data[2]]["fd"] = fd;
    //trr("IRN: returning 1!","yellow");
    return 1;
}

static int ValidatePeer(int fd, mixed data){
    string ip = explode(socket_address(fd)," ")[0];
    mixed tmp;
    if(!irn_enabled) return 0;
    //trr("IRN: hit ValidatePeer","green");
    if(member_array(ip,ok_ips) == -1){
        server_log("IRN: bad ip: "+ip);
        server_log("ok_ips: "+identify(ok_ips));
        return 0;
    }
    if(!data || !arrayp(data) || !irn_connections[data[2]]){ 
        trr("IRN: no such connection.");
        return 0;
    }
    if(member_array(data[2], keys(routers)) == -1){
        trr("IRN: unknown peer");
        return 0;
    }
    if(sizeof(tmp = irn_connections[data[2]])){
        //tc("validation. fd: "+fd+", tmp: "+identify(tmp));
        if(tmp["fd"] && tmp["fd"] != fd){
            trr("validation. fd: "+fd+", tmp: "+identify(tmp),"red");
            trr("LOLOLOLOLOL","red");
            return 0;
        }
    }
    //else tc("validation. fd: "+fd+", no tmp? "+identify(tmp));
    //trr("returning 1!","green");
    return 1;
}

static string id_mud(int fd){
    string *ret = ({});
    foreach(mixed element in keys(irn_connections)){
        if(irn_connections[element]["fd"] == fd) ret += ({ element });
    }
    return implode(ret,", ");
}

void irn_clear(){
    if(sizeof(irn_connections))
        foreach(mixed key, mixed val in irn_connections){
            if(!key || !sizeof(key)) continue;
            if(!irn_connections[key] || !irn_connections[key]["fd"]) continue;
            //tc("irn_clear closing "+irn_connections[key]["fd"]);
            this_object()->close_connection(irn_connections[key]["fd"]);
            irn_connections[key]["connected"] = 0;
        }
    if(sizeof(irn_sockets))
        foreach(mixed key, mixed val in irn_sockets){
            if(!key || !sizeof(key)) continue;
            //tc("irn_clear (2) closing "+key);
            this_object()->close_connection(key);
        }

    irn_connections = ([]);
    irn_sockets = ([]);

    foreach(string key, mixed val in routers){
        if(!key || !sizeof(key)) continue;
        if(key == my_name){
            //map_delete(routers, key);
            continue;         }
        //ok_ips += ({ routers[key]["ip"] });
        if(!irn_connections[key]){
            //irn_connections[key] = (["blocking" : 1, "type": MUD, "fd": -1, "data" : 0 ]);
        }
    }
    save_object(SAVE_ROUTER);
}

varargs void irn_setup(int clear, string whom){
    mapping which = ([]);
    if(!PingMap) PingMap = ([]);
    //mapping tmpinfo = this_object()->query_mudinfo();

    server_log("%^RESET%^irn_setup("+clear+(whom ? (", "+whom ):"")+"): "+get_stack());
    foreach(mixed key, mixed val in PingMap){
        if(key == my_name || member_array(key,keys(routers)) == -1) map_delete(PingMap,key);
    }

    if(!irn_enabled) return;

    if(clear){
        if(sizeof(irn_connections))
            foreach(mixed key, mixed val in irn_connections){
                if(!irn_connections[key] || !irn_connections[key]["fd"]) continue;
                irn_connections[key]["connected"] = 0;
                //tc("irn_setup closing "+irn_connections[key]["fd"]);
                this_object()->close_connection(irn_connections[key]["fd"]);
            }
        if(sizeof(irn_sockets))
            foreach(mixed key, mixed val in irn_sockets){
                //tc("irn_setup closing "+key);
                this_object()->close_connection(key);
            }
        irn_clear();
    }

    if(whom) which = ([ whom : routers[whom] ]);
else which = routers;
//trr("irn_setup().which: "+identify(which));

foreach(string key, mixed val in which){
    //mapping owned_muds = ([]);
    if(key == my_name){
        //map_delete(routers, key);
        continue;
    }
    ok_ips += ({ routers[key]["ip"] });
    server_log("ok_ips: "+identify(ok_ips));
    if(!irn_connections[key]){
        foreach(mixed key2, mixed val2 in mudinfo){
            //if(val2["router"] == key) this_object()->disconnect_mud(key2, 1);
            if(val2["router"] && val2["router"] == key &&
              !mudinfo[key2]["disconnect_time"]){
                trr(key+" is down, disconnecting: "+key2,"red");
                this_object()->disconnect_mud(key2, 1);
            }
        }
    }
}

foreach(string name in keys(which)){
    int tmp_fd, sockstat;

    if(name == my_name){
        //map_delete(routers, key);
        continue;
    }
    trr("About to try connecting to: "+identify(name));

    tmp_fd = socket_create(MUD, "irn_read_callback","irn_close_callback"); 
    if(tmp_fd < 0){
        trr("irn: Couldn't create socket. errorcode: "+socket_error(tmp_fd));
        return;
    }

    irn_connections[name] = ([]);
    sockstat = socket_bind(tmp_fd, 0);
    trr("socket_bind: "+sockstat);

    if(sockstat < 0){
        trr("irn: Couldn't bind socket. errorcode: "+socket_error(sockstat));
        return;
    }

    irn_connections[name]["fd"] = tmp_fd;
    irn_sockets[tmp_fd] = (["name" : name]);

    sockstat = socket_connect(irn_connections[name]["fd"], routers[name]["ip"]+" "+
      routers[name]["port"], "irn_read_callback", "irn_write_callback");
    trr("socket_connect: "+sockstat);

    if(sockstat < 0){
        trr("irn: Couldn't connect to "+name+", errorcode: "+socket_error(sockstat));
        this_object()->close_connection(irn_connections[name]["fd"]);
        irn_connections[name]["fd"] = -1;
    }
    this_object()->Report();
    begin_socket_handoff(tmp_fd);
    trr("%^B_WHITE%^Initiating eventSendStartup callout for "+tmp_fd);
    call_out( "eventSendStartup", 5, tmp_fd);
}
}

varargs void eventSendStartup(int fd){
    mixed *targets = ({});
    if(!irn_enabled) return;
    trr("irn: hit eventSendStartup","yellow");
    trr("irn: hit eventSendStartup: "+get_stack(),"green");
    if(!fd) targets = keys(irn_connections);
    else if(irn_sockets[fd] && irn_sockets[fd]["name"]) targets = ({ irn_sockets[fd]["name"] });
    else targets = keys(irn_connections);
    if(!sizeof(targets)){
        call_out( "irn_setup", 5, 1 );
        return;
    }
    this_object()->Report();
    foreach(mixed element in targets){
        if(irn_connections[element]["connected"]) continue;
        irn_connections[element]["connected"] = 1;

        trr("irn: sending statup to "+element+" on fd "+irn_connections[element]["fd"]);
        write_data(irn_connections[element]["fd"],
          ({
            "irn-startup-req",
            5,
            my_name,
            "foo",
            element,
            "bar",
            ([ "client_password" : my_password, "server_password" : routers[element]["password"] ])
        })
      );
    call_out( "SendWholeList", 10, irn_connections[element]["fd"]);
}
}

static void irn_write_callback(int fd){
    //trr("hit irn_write_callback for fd"+fd);
}

static void irn_close_callback(int fd){
    if(!irn_enabled) return;
    irn_connections[id_mud(fd)]["fd"] = -1;
    irn_connections[id_mud(fd)]["connected"] = 0;
    map_delete(irn_sockets, fd);
    trr("I'm wanting to close "+id_mud(fd)+" on fd"+fd+" now.");
    call_out( "eventSendStartup", 5);
}

static void irn_read_callback(int fd, mixed data){
    int i;
    string tmp="";
    mapping MudList = ([]);
    if(!irn_enabled) return;
    if(this_object()->query_mudinfo())
        MudList = this_object()->query_mudinfo();
    else MudList = ([ "wt" : "f" ]);
    //trr("irn: hit irn_read_callback for fd"+fd+", "+id_mud(fd));
    if(!data || !sizeof(data)){
        trr("irn: no data");
    }
    else {
        if(bufferp(data)){
            //trr("data buffer: "+identify(read_buffer(data)));
            for(i=0;i<sizeof(data);i++){
                tmp += sprintf("%c",data[i]);
            }
        }
        else {
            //trr("data: "+identify(data));
        }
    }
    //trr("tmp: "+tmp);
    //trr("mudinfo keys: "+identify(keys(MudList)));
    switch(data[0]){
    case "irn-startup-req" :
        trr("IRN got a startup request on fd"+fd);
        if(!GoodPeer(fd, data)) return;
        trr("IRN startup from "+data[2]+" accepted.");
        if(sizeof(irn_connections)){
            foreach(mixed key, mixed val in irn_connections){
                //if(irn_connections[key] && irn_connections[key]["connected"]) return;
                //trr("IRN: looking at: "+key+" "+identify(val),"green");
                if(key == data[2] && fd != irn_connections[key]["fd"]){
                    //this_object()->close_connection(irn_connections[key]["fd"]);
                    map_delete(irn_sockets, irn_connections[key]["fd"]);
                    //map_delete(irn_connections, key);
                }
            }
        }
        if(sizeof(irn_sockets))
            foreach(mixed key, mixed val in irn_sockets){
                trr("IRN: looking at: "+key+" "+identify(val),"cyan");
                if(val["name"] == data[2]){
                    //this_object()->close_connection(key);
                    //map_delete(irn_connections, data[2]);
                    if(fd != key){
                        map_delete(irn_sockets, key);
                        //this_object()->close_connection(key);
                    } 
                }
            }
        if(!irn_connections[data[2]]) irn_connections[data[2]] = ([]);
        irn_connections[data[2]]["fd"] = fd;
        irn_connections[data[2]]["connected"] = 1;
        trr("irn_connections[\""+data[2]+"\"][\"fd\"]: "+irn_connections[data[2]]["fd"]);
        irn_sockets[fd] = ([ "name" : data[2] ]);
    call_out( "SendWholeList", 5, fd);
    trr(data[2]+" has joined IRN on socket "+fd);
    PingMap[data[2]] = time();
    trr("%^B_GREEN%^STARTUP COMPLETE: irn_connections: "+identify(irn_connections));
    trr("%^B_GREEN%^STARTUP COMPLETE: irn_sockets: "+identify(irn_sockets));
    this_object()->clean_ghosts();
    return;
case "irn-mudlist-altered" :
    //trr("irn: received irn-mudlist-altered");
    if(!ValidatePeer(fd, data)) {
        trr("irn: peer failed validation.");
        this_object()->close_connection(fd);
        if(irn_sockets[fd]) map_delete(irn_sockets, fd);
        irn_checkstat();
        //if(irn_connections[data[2]]) map_delete(irn_connections, data[2]);
        return;
    }
    //trr("got new delta. sending to router.");
    PingMap[data[2]] = time();
    this_object()->ReceiveList(data[7],"mudlist",data[2]);
    break;
case "irn-mudlist-delta" :
    //trr("irn: got mudlist delta");
    if(!ValidatePeer(fd, data)) {
        trr("irn: peer failed validation.");
        this_object()->close_connection(fd);
        if(irn_sockets[fd]) map_delete(irn_sockets, fd);
        irn_checkstat();
        //if(irn_connections[data[2]]) map_delete(irn_connections, data[2]);
        return;
    }
    //trr("irn: got new delta. sending to router.");
    PingMap[data[2]] = time();
    this_object()->ReceiveList(data[7],"mudlist",data[2]);
    break;
case "irn-chanlist-altered" :
    trr("irn: received irn-chanlist-altered");
    if(!ValidatePeer(fd, data)) {
        trr("irn: peer failed validation.");
        this_object()->close_connection(fd);
        if(irn_sockets[fd]) map_delete(irn_sockets, fd);
        irn_checkstat();
        //if(irn_connections[data[2]]) map_delete(irn_connections, data[2]);
        return;
    }
    //trr("got new delta. sending to router.");
    PingMap[data[2]] = time();
    this_object()->ReceiveList(data[7],"chanlist",data[2]);
    break;
case "irn-chanlist-delta" :
    trr("irn: got chanlist delta");
    if(!ValidatePeer(fd, data)) {
        trr("irn: peer failed validation.");
        this_object()->close_connection(fd);
        if(irn_sockets[fd]) map_delete(irn_sockets, fd);
        irn_checkstat();
        //if(irn_connections[data[2]]) map_delete(irn_connections, data[2]);
        return;
    }
    //trr("irn: got new delta. sending to router.");
    trr("this_object()->ReceiveList("+identify(data[7])+", chanlist, "+identify(data[2]));
    PingMap[data[2]] = time();
    this_object()->ReceiveList(data[7],"chanlist",data[2]);
    break;
case "irn-data" :
    if(!ValidatePeer(fd, data)) {
        trr("irn-data: peer on fd"+fd+" failed validation.");
        this_object()->close_connection(fd);
        if(irn_sockets[fd]) map_delete(irn_sockets, fd);
        irn_checkstat();
        //if(irn_connections[data[2]]) map_delete(irn_connections, data[2]);
        return;
    }
    PingMap[data[2]] = time();
    //trr("irn: got new data, type: "+data[0]+". sending to router.");
    if(convert_channel && !strsrch(data[6][0],"chan") && strsrch(data[6][0],"chan-user")){
        if(irn_sockets[fd] && irn_sockets[fd]["name"] && chan_conv[irn_sockets[fd]["name"]]){
            //trr("chan_conv");
            foreach(string key, string val in chan_conv[irn_sockets[fd]["name"]]){
                //trr("data[6][6]: "+data[6][6]);
                data[6][6] = replace_string(data[6][6],key,val);
                //tc("data[6][6]: "+data[6][6], "cyan");
            }
        }
    } 
    trr("irn_read_callback sending read_callback("+
      identify(data[6][2])+", "+identify(data[6])+")");
    this_object()->read_callback(data[6][2],data[6]);
    break;
case "irn-ping" :
    //trr("irn: got ping from "+data[2]+" on fd"+fd);
    if(!ValidatePeer(fd, data)){
        trr("irn: "+data[2]+" failed validation.");
        this_object()->close_connection(fd);
        if(irn_sockets[fd]) map_delete(irn_sockets, fd);
        irn_checkstat();
        return;
    }
    PingMap[data[2]] = time();
    //trr("irn: got ping from "+data[2]+" on fd"+fd+", PingMap: "+
    //identify(PingMap)+", time: "+time());
    break;
default : 
    if(!ValidatePeer(fd, data)) {
        trr("irn: Invalid peer: "+identify(socket_status(fd)),"red");
        this_object()->close_connection(fd);
        if(irn_sockets[fd]) map_delete(irn_sockets, fd);
        if(irn_connections[data[2]]) map_delete(irn_connections, data[2]);
        return;
    }
    //trr("irn: got i3 data, type: "+data[1][0]+". sending to router.");
    PingMap[data[2]] = time();
    if(arrayp(data[6])) this_object()->read_callback(data[6][2],data[6]);
    return;
}
}

static varargs void SendList(mixed data, int fd, string type){
    int *targets = ({});
    string *cmuds = this_object()->query_connected_muds();
    mixed *outbound = ({});
    mapping tmp = ([]);
    if(!type || !sizeof(type)) type = "irn-mudlist-delta";
    if(type == "mudlist") type = "irn-mudlist-delta";
    if(type == "chanlist") type = "irn-chanlist-delta";
    //trr("list type: "+type);
    //trr("%^RESET%^irn: trying to send list: "+identify(keys(data)));
    if(!irn_enabled) return;
    if(!mapp(data)){
        trr("irn: Tried to send a non-map. guilty stack: "+get_stack(),"cyan");
        return;
    }
    //trr("1");
    tmp = copy(data);
    if(mapp(tmp)){
        foreach(mixed key, mixed val in tmp){
            if(!mapp(val)){
                trr("irn: Non mapping val. Key: "+key+". stack: "+get_stack(),"yellow");
                trr("irn: guilty stack: "+get_stack(),"cyan");
            }
            else {
                if(type == "mudlist"){
                    //if(member_array(key,cmuds) == -1) map_delete(data,key);
                    if(mudinfo[key] && mudinfo[key]["router"] &&
                      mudinfo[key]["router"] != my_name){
                        trr("Not sending "+key+"."+mudinfo[key]["router"]);
                        map_delete(data,key);
                    }
                }
                if(type == "chanlist"){
                    true();
                }
            }
        }
    }
    //trr("2");
    if(!fd) targets = keys(irn_sockets);
    else targets = ({ fd });
    foreach(int router in targets){
        if(!irn_sockets[router] || !mapp(irn_sockets[router])) continue;
        //trr("3");
        if(type == "irn-chanlist-delta"){
            //print_long_string(find_player("cratylus"),"I'm trying to send this: "+identify(data)+" to "+router);
            //continue;
        }
        write_data(router, ({
            type,
            5,
            my_name,
            0,
            irn_sockets[router]["name"],
            0,
            time(),
            data
          }) );
    }
}

varargs void SendWholeList(int fd, string type){
    mapping tmp = ([]);
    int i=1;

    this_object()->clean_ghosts();

    if(!type || !sizeof(type)) type = "mudlist";
    if(!irn_enabled) return;
    if(!fd) fd = 0;
    trr("SendWholeList type: "+type);
    switch(type){
    case "mudlist" :
        tmp = this_object()->query_mudinfo();
        break;
    case "chanlist" :
        tmp = this_object()->query_chaninfo();
        break;
    default :
        trr("irn SendWholeList error: bad type "+type);
        return;
    }
    foreach(mixed key, mixed val in tmp){
        //trr("irn: Sending "+key,"white");
        if(type == "mudlist") if(mapp(val)){
                //trr("scheduling "+key,"red");
                i++;
                call_out( (: SendList :), i, ([ key : val ]), fd, type );
                //SendList( ([ key : val ]), fd, type );
            }
        if(type == "chanlist"){
            foreach(mixed key2, mixed val2 in val){
                i++;
                call_out( (: SendList :), i, ([ key : ([ key2 : val2 ]) ]), fd, type );
                //SendList( ([ key : ([ key2 : val2 ]) ]), fd, type );
            }
        }
    }
}

static void SendMessage(mixed data){
    string routername, cible;
    mapping tmpinfo = this_object()->query_mudinfo();
    mixed tmpdata; 
    int *but;
    trr("irn: received SendMessage call","white");
    if(!irn_enabled) return;
    if(tmpinfo && sizeof(data) > 4 && sizeof(tmpinfo[data[4]]) && 
      cible = tmpinfo[data[4]]["router"]){
        if(mapp(tmpdata = this_object()->query_irn_connections()[cible])){
            but = ({ tmpdata["fd"] });
        }
    }
    if(!but || !sizeof(but)) but = keys(irn_sockets);
    foreach(int router in but){
        trr("irn sending "+data[0]+" for "+identify(data[4])+
          " to "+identify(irn_sockets[router]));
        tmpdata = copy(data);
        if((convert_channel || convert_channel2) && !strsrch(tmpdata[0],"chan") && strsrch(tmpdata[0],"chan-user")){
            routername = irn_sockets[router]["name"];
            //trr("looking at "+routername+" for conversion.");
            if(member_array(routername, keys(chan_conv)) != -1){
                //trr("yep. it's: "+identify(chan_conv[routername]));
                foreach(string key, string val in chan_conv[routername]){
                    if(convert_channel){
                        //trr("conv1: tmpdata[6]: "+tmpdata[6]);
                        tmpdata[6] = replace_string(tmpdata[6],val,key);
                        //trr("conv1: tmpdata: "+identify(tmpdata));
                    }
                    if(convert_channel2){
                        //trr("conv2: tmpdata[6]: "+tmpdata[6]);
                        tmpdata[6] = replace_string(tmpdata[6],key,val);
                        //trr("conv2: tmpdata: "+identify(tmpdata));
                    }

                }
            }
        }

        write_data(router, ({
            "irn-data",
            5,
            my_name,
            0,
            "*",
            0,
            tmpdata
          }) );
    }
}

string Report(){
    string ret = "IRN: connections: ";
    if(sizeof(irn_connections))
        foreach(mixed key, mixed val in irn_connections){
            if(!key) continue;
            if(!irn_connections[key]) return;
            ret += key+":"+irn_connections[key]["fd"]+" ";
        }
    ret += "\n";
    ret += "IRN: sockets: ";
    foreach(mixed key, mixed val in irn_sockets){
        int sstat = -1;
        string statmess = "DISCONNECTED";
        if(irn_connections[val["name"]]){
            sstat = irn_connections[val["name"]]["fd"];
            if(sstat > 0 && socket_status(key)){
                string *netstat = socket_status(key);
                statmess = netstat[1]+" ";
                statmess += last_string_element(netstat[3],".");
                statmess +=":";
                statmess += last_string_element(netstat[4],".") + " ";
            }
            ret += key +":"+val["name"]+":"+statmess+" ";
        }
        ret += "\n";
    }
    ret += "ok_ips: "+identify(ok_ips)+"\n";
    //trr("irn: irn_connections: "+identify(irn_connections),"green");
    //trr("irn: irn_sockets: "+identify(irn_sockets),"cyan");
    foreach(mixed element in keys(irn_sockets)){
        //trr("socket_status("+element+"): "+identify(socket_status(element)),"green");
    }
    foreach(mixed key, mixed val in PingMap){
        if(key == my_name) continue;
        ret += key+" lastping: "+time_elapsed(time()-val)+" ago\n";
    }
    return ret;
}

static void begin_socket_handoff(int fd){
    object rsock = find_object(RSOCKET_D);
    if(!irn_enabled) return;
    if(!rsock) rsock = load_object(RSOCKET_D);
    if(!rsock){
        trr("IRN: Socket handoff failed. Couldn't load RSOCKET_D");
        return;
    }
    socket_release(fd, rsock, "complete_socket_handoff");
}

mapping query_irn_sockets(){
    validate();
    return copy(irn_sockets);
}

mapping query_irn_connections(){
    validate();
    return copy(irn_connections);
}

int query_irn_enabled(){
    if(irn_enabled) return 1;
    return 0;
}

int toggle_irn(int x){
    validate();
    if(x) irn_enabled = 1;
    else irn_enabled = 0;
    return irn_enabled;
}

varargs mixed irn_ping(mixed target, int code){
    //return 1010011;
    if(!intp(target)){
        target = irn_connections[target];
        if(target && target["fd"]) target = target["fd"];
        else {
            //trr("badping: "+identify(target));
            return 0;
        }
    }

    if(!irn_sockets[target]) return 0;
    if(!code) code = time();

    //trr("Sending ping to: "+irn_sockets[target]["name"]+" on fd"+target,"green");
    write_data(target, ({
        "irn-ping",
        5,
        my_name,
        0,
        irn_sockets[target]["name"],
        0,
        time(),
        code
      }) );

    return 1;
}

int send_pings(){
    foreach(string node in keys(routers)){
        if(node == my_name) continue;
        irn_ping(node);
    }
}
