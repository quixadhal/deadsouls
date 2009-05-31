// This file written mostly by Tim Johnson (Tim@TimMUD)
void check_discs();

string *banned_muds = ({});
mapping bad_connects = ([]);

static void process_startup_req(int protocol, mixed info, int fd){
    // Handles startup stuff.
    //  Loads info into newinfo mapping.
    //  Error if mud is already connected.
    mapping newinfo;
    string site_ip;
    string *brethren = ({});
    // router name is info[4], I'll just pretend I'm every router though, ha-ha!
    // also, should verify that all the fields are the right type

    check_discs();
    server_log("%^BLUE%^process_startup_req: protocol="+protocol+", mud="+info[2]);

    if(member_array(info[2], banned_muds) != -1) {
        server_log("%^RED%^"+info[2]+" denied. reason: banned.");
        trr("---\n","blue");
        return;
    }

    if(!info[2] || !sizeof(info[2])) {
        server_log("%^RED%^"+info[2]+" denied. reason: null name");
        trr("---\n","blue");
        return;
    }

    foreach(mixed element in keys(mudinfo)){
        if(member_array(element,keys(connected_muds)) == -1) continue;
        if(mudinfo[element] && mudinfo[element]["ip"] == clean_fd(socket_address(fd)))
            brethren += ({ element });
    }
    trr("Number of muds with the same ip: "+sizeof(brethren));
    if(sizeof(brethren) > MAXMUDS && !mudinfo[info[2]]){
        write_data(fd,({ "error", 5, router_name, 0, info[2], 0,
                    "not-allowed", "Too many muds from your IP.", info }) );
        return;
    }
    if(member_array(fd,keys(this_object()->query_connected_fds())) != -1){
        string existing_mud = this_object()->query_connected_fds()[fd];
        trr("Socket status: "+identify(socket_status(fd)),"red");
        server_log("%^RED%^FD CONFLICT, NEW MUD: "+info[2]+", FD: "+fd);
        server_log("%^RED%^FD CONFLICT, OLD MUD: "+existing_mud+", FD: "+fd);
        trr("---\n","blue");
        if(mudinfo[existing_mud]){
            mudinfo[existing_mud]["disconnect_time"] = time();
        }
        if(!existing_mud || existing_mud != info[2]){
        map_delete(connected_muds, existing_mud);
        schedule_broadcast(existing_mud);
        }
    }

    if(sizeof(info)<18){ 
        // smallest protocol is protocol 1/2 which have size 18
        write_data(fd,({
                    "error",
                    5,
                    router_name,
                    0,
                    info[2],
                    0,
                    "bad-pkt",
                    "Bad startup_req packet",
                    info
                    }));
        return;
    }
    site_ip=socket_address(fd);
    site_ip = clean_fd(site_ip);
    newinfo = ([
            "name":info[2],
            "ip":site_ip,
            "connect_time":time(),
            "disconnect_time":0,
            "password":info[6],
            "old_mudlist_id":info[7],
            "old_chanlist_id":info[8],
            "player_port":info[9],
            "imud_tcp_port":info[10],
            "imud_udp_port":info[11],
            "mudlib":info[12],
            "base_mudlib":info[13],
            "driver":info[14],
            "mud_type":info[15],
            "open_status":info[16],
            "protocol":protocol,
            "restart_delay":-1,
            "router" : my_name,
            ]);

    if(protocol == 2 && sizeof(info) == 20 ) protocol = 3;

    switch(protocol){
        case 1:
        case 2:
            if(sizeof(info)!=18){
                server_log("error: wrong size packet. Got: "+sizeof(info)+", wanted 18");
                trr("---\n","blue");
                write_data(fd,({
                            "error",
                            5,
                            router_name,
                            0,
                            info[2],
                            0,
                            "bad-pkt",
                            "Bad startup_req packet",
                            info
                            }));
                return;
            }
            newinfo["services"]=info[17];
            newinfo["admin_email"]="Unknown"; // only in protocol 3
            newinfo["other_data"]=0; // only in protocol 3
            break;
        case 3:
            if(sizeof(info)!=20){
                server_log("error. wrong size packet. Got: "+sizeof(info)+", wanted 20");
                trr("---\n","blue");

                write_data(fd,({
                            "error",
                            5,
                            router_name,
                            0,
                            info[2],
                            0,
                            "bad-pkt",
                            "Bad startup_req packet",
                            info
                            }));
                return;
            }
            newinfo["admin_email"]=info[17];
            newinfo["services"]=info[18];
            newinfo["other_data"]=info[19];
            break;
        default:
            server_log("error. I dunno what.");

            write_data(fd,({
                        "error",
                        5,
                        router_name,
                        0,
                        info[2], // mud name
                        0,
                        "not-imp",
                        "unknown protocol: "+protocol,
                        info
                        }));
            return;
    }
    // Check valid values...
    if(
            !stringp(newinfo["open_status"]) ||
            !stringp(newinfo["admin_email"]) ||
            !stringp(newinfo["mud_type"]) ||
            !stringp(newinfo["driver"]) ||
            !stringp(newinfo["base_mudlib"]) ||
            !stringp(newinfo["mudlib"]) ||
            !mapp(newinfo["services"]) ||
            !intp(newinfo["imud_udp_port"]) ||
            !intp(newinfo["imud_tcp_port"]) ||
            !intp(newinfo["player_port"]) ||
            !intp(newinfo["password"]) ||
            !intp(newinfo["old_chanlist_id"]) ||
            !intp(newinfo["old_mudlist_id"]) ||
            (!mapp(newinfo["other_data"]) && newinfo["other_data"]!=0)
      ){
        write_data(fd,({
                    "error",
                    5,
                    router_name,
                    0,
                    info[2],
                    0,
                    "bad-pkt",
                    "Bad startup_req packet",
                    info
                    }));
        return;
    }
    if(!undefinedp(connected_muds[info[2]])){
        // if MUD is already connected
        trr("ROUTER_D: mud already connected on fd"+connected_muds[info[2]],"red");
        trr("ROUTER_D: Xref back: mud on fd"+connected_muds[info[2]]+" is "+
                "supposed to be "+this_object()->query_connected_fds()[this_object()->query_connected_muds()[info[2]]],"red");
        server_log("mud already connected");
        trr("---\n","blue");
        if(this_object()->query_mudinfo()[info[2]]["ip"] != explode(socket_address(fd)," ")[0] ||
                mudinfo[info[2]]["password"] != newinfo["password"]){
            write_data(fd,({
                        "error",
                        5,
                        router_name,
                        0,
                        info[2], // mud name
                        0,
                        "bad-proto", // doesn't seem to me like it should be bad-proto...
                        // see what the official one uses for this
                        // it might just boot the earlier MUD off?
                        "MUD already connected", // Error message
                        info
                        }));

            //This message to the already-connected mud serves 
            //to alert the legit mud that something may be wrong, and
            //also will trigger a closure of the socket if it turns out
            //to actually be in a zombie state.
            write_data(connected_muds[info[2]],({
                        "error",
                        5,
                        router_name,
                        0,
                        info[2], // mud name
                        0,
                        "bad-mojo", 
                        "Another mud is trying to be you.", 
                        ({ explode(socket_address(fd)," ")[0], info[12], info[13], info[17], info[9] })
                        }));
            return;
        }
        if(this_object()->query_mudinfo()[info[2]]["ip"] == explode(socket_address(fd)," ")[0] &&
                mudinfo[info[2]]["password"] == newinfo["password"] ){
            server_log("Since it's the same ip and password, I'll remove the current connection");
            if(mudinfo[info[2]]) mudinfo[info[2]]["disconnect_time"] = time();
            map_delete(connected_muds, info[2]);
            schedule_broadcast(info[2]);
        }
    }

    if(mudinfo[info[2]] && sizeof(mudinfo[info[2]]) && mudinfo[info[2]]["password"] != newinfo["password"]){
        // if MUD is already known, not connected, and wrong password
        if(newinfo["ip"]==mudinfo[info[2]]["ip"]){
            // same IP as last time and it isn't Dead Souls
            server_log("Wrong password, but right IP");
            trr("Expected "+mudinfo[info[2]]["password"]+
                    " but got "+newinfo["password"]);
            write_data(fd,({
                        "error",5,router_name,0,info[2],0,
                        "warning", // nothing in error summary that seems applicable?
                        // Change later if I find a better error code
                        "wrong password, but I'll allow you since it's the same IP as last time",0
                        }));
        }
        else{
            trr("wrong password, and from a new IP","red");
            trr("---\n","blue");
            if(!bad_connects[newinfo["ip"]]) bad_connects[newinfo["ip"]] = 1;
            else bad_connects[newinfo["ip"]] = bad_connects[newinfo["ip"]] + 1;
            if(bad_connects[newinfo["ip"]] > 100){
                trr("ROUTER_D: "+newinfo["ip"]+" blacklisted!","red");
                this_object()->AddBlacklistedMud(newinfo["ip"]);
                bad_connects[newinfo["ip"]] = 0;
            }
            server_log("%^RED%^WRONG PASSWORD, AND FROM A NEW IP");
            write_data(fd,({
                        "error",
                        5,
                        router_name,
                        0,
                        info[2], // mud name
                        0,
                        "not-allowed", // Change later!
                        "wrong password, and from a new IP", // Error message
                        info
                        }));
            return;
        }
    }
    if(!mudinfo[info[2]] || !newinfo["password"] || mudinfo[info[2]]["password"] != newinfo["password"] ){
        // if new MUD, assign it a password
        newinfo["password"]=random_numbers(9,1);
        trr("ROUTER_D: Assigning password "+newinfo["password"],"white");
        // Change this maybe... see if the password is supposed to be in a certain range
    }
    else {
        trr("ROUTER_D: password: Known: "+mudinfo[info[2]]["password"]+", current: "+newinfo["password"],"green");
    }
    // MUD should be okay at this point.
    mudinfo[info[2]]=newinfo; // update the mudinfo
    connected_muds[info[2]] = fd; // add this MUD to list of connected muds
    send_startup_reply(info[2]); // reply to MUD
    mudinfo_update_counter++;
    mudinfo_updates[info[2]]=mudinfo_update_counter;
    send_mudlist_updates(info[2], newinfo["old_mudlist_id"]);
    send_full_mudlist(info[2]);
    broadcast_mudlist(info[2]);
    broadcast_chanlist("foo",info[2]);
    if(bad_connects[newinfo["ip"]]) bad_connects[newinfo["ip"]] = 0;
    if(member_array("channel", keys(newinfo["services"])) != -1){

    }
    else {
        trr("It looks like "+info[2]+" doesn't have a channel service?!?","blue");
    }
    trr(timestamp()+" process_startup_req: for mud: "+info[2]+" complete.\n---\n","blue");
}
