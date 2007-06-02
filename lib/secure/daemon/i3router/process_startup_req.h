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

    //trr("info: "+identify(info));
    //trr("Incoming data from fd("+fd+"), address "+socket_address(fd)+".","blue");
    check_discs();
    //trr("The known status of that fd is "+identify(socket_status(fd)),"blue");
    //trr("muds on that fd: "+identify(filter(keys(this_object()->query_connected_muds()), (: this_object()->query_connected_muds()[$1] == $(fd) :) )),"blue");
    trr(timestamp()+" process_startup_req: protocol="+protocol+", mud="+info[2],"blue");
    log_file("router/server_log",timestamp()+" process_startup_req: protocol="+protocol+", mud="+info[2]+"\n");

    if(member_array(info[2], banned_muds) != -1) {
	trr(timestamp()+" "+info[2]+" denied. reason: banned.\n");
	trr("---\n","blue");
	log_file("router/server_log",timestamp()+" "+info[2]+" denied. reason: banned.\n");
	return;
    }

    if(!info[2] || !sizeof(info[2])) {
	trr(timestamp()+" "+info[2]+" denied. reason: null name.\n");
	trr("---\n","blue");
	log_file("router/server_log",timestamp()+" "+info[2]+" denied. reason: null name\n");
	return;
    }

    //    brethren = 
    //filter(keys(mudinfo), 
    //(: mudinfo[$1]["ip"] == 
    //clean_fd(socket_address($(fd))) :) ); 
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

    if(fd && member_array(fd,keys(this_object()->query_connected_fds())) != -1){
	string existing_mud = this_object()->query_connected_fds()[fd];
	trr("FD CONFLICT, NEW MUD: "+info[2]+", FD: "+fd,"red");
	trr("FD CONFLICT, EXISTING MUD: "+existing_mud+", FD: "+fd,"red");
	trr("Socket status: "+identify(socket_status(fd)),"red");
	trr("---\n","blue");
	log_file("router/server_log","FD CONFLICT, MUD: "+info[2]+", FD: "+fd+"\n");
	if(mudinfo[existing_mud]) mudinfo[existing_mud]["disconnect_time"] = time();
	map_delete(connected_muds, existing_mud);
	//broadcast_mudlist(existing_mud);
	schedule_broadcast(existing_mud);
    }

    if(sizeof(info)<18){ 
	// smallest protocol is protocol 1/2 which have size 18
	//trr("THIS SHOULDNT BE HERE");
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
    //trr("fd is:" +fd,"cyan");
    site_ip=socket_address(fd);
    site_ip = clean_fd(site_ip);
    //trr("site_ip: "+site_ip,"cyan");
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
    //trr("newinfo: "+identify(newinfo));

    if(protocol == 2 && sizeof(info) == 20 ) protocol = 3;

    switch(protocol){
    case 1:
    case 2:
	if(sizeof(info)!=18){
	    trr("error: wrong size packet. Got: "+sizeof(info)+", wanted 18","red");
	    trr("---\n","blue");
	    log_file("router/server_log",timestamp()+" error: wrong size packet. Got: "+sizeof(info)+", wanted 18\n");
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
	    trr("error. wrong size packet. Got: "+sizeof(info)+", wanted 20","red");
	    trr("---\n","blue");
	    log_file("router/server_log",timestamp()+" error. wrong size packet. Got: "+sizeof(info)+", wanted 20\n");

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
	log_file("router/server_log",timestamp()+" error. I dunno what.\n");

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
    if(connected_muds[info[2]]){
	// if MUD is already connected
	trr("ROUTER_D: mud already connected on fd"+connected_muds[info[2]],"red");
	trr("ROUTER_D: Xref back: mud on fd"+connected_muds[info[2]]+" is "+
	  "supposed to be "+this_object()->query_connected_fds()[this_object()->query_connected_muds()[info[2]]],"red");
	trr("---\n","blue");
	log_file("router/server_log",timestamp()+" mud already connected\n");
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
	    return;
	}
	if(this_object()->query_mudinfo()[info[2]]["ip"] == explode(socket_address(fd)," ")[0] &&
	  mudinfo[info[2]]["password"] == newinfo["password"] ){
	    trr("Since it's the same ip and password, I'll remove the current connection.");
	    log_file("router/server_log",timestamp()+" Since it's the same ip and password, I'll remove the current connection\n");
	    //this_object()->remove_mud(info[2],1);
	    if(mudinfo[info[2]]) mudinfo[info[2]]["disconnect_time"] = time();
	    map_delete(connected_muds, info[2]);
	    //broadcast_mudlist(info[2]);
	    schedule_broadcast(info[2]);
	}
    }

    if(mudinfo[info[2]] && sizeof(mudinfo[info[2]]) && mudinfo[info[2]]["password"] != newinfo["password"]){
	// if MUD is already known, not connected, and wrong password
	if(newinfo["ip"]==mudinfo[info[2]]["ip"] && info[2] != "Dead Souls"){
	    // same IP as last time and it isn't Dead Souls
	    trr("Wrong password, but right IP","green");
	    log_file("router/server_log",timestamp()+" Wrong password, but right IP\n");
	    write_data(fd,({
		"error",5,router_name,0,info[2],0,
		"warning", // nothing in error summary that seems applicable?
		// Change later if I find a better error code
		"wrong password, but I'll allow you since it's the same IP as last time",0
	      }));
	}
	else{
	    trr("wrong password, and from a new IP (or a mud named Dead Souls)","red");
	    trr("---\n","blue");
	    if(!bad_connects[newinfo["ip"]]) bad_connects[newinfo["ip"]] = 1;
	    else bad_connects[newinfo["ip"]] = bad_connects[newinfo["ip"]] + 1;
	    if(bad_connects[newinfo["ip"]] > 100){
		trr("ROUTER_D: "+newinfo["ip"]+" blacklisted!","red");
		this_object()->AddBlacklistedMud(newinfo["ip"]);
		bad_connects[newinfo["ip"]] = 0;
	    }
	    log_file("router/server_log",timestamp()+" WRONG PASSWORD, AND FROM A NEW IP\n");
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
	newinfo["password"]=random_numbers(9,1);	trr("ROUTER_D: Assigning password "+newinfo["password"],"white");
	//trr("Ok. this is the password: "+newinfo["password"],"white");
	// Change this maybe... see if the password is supposed to be in a certain range
    }
    else {
	trr("ROUTER_D: password: Known: "+mudinfo[info[2]]["password"]+", current: "+newinfo["password"],"green");
    }
    // MUD should be okay at this point.
    //trr("about to update the mudinfo...","white");
    mudinfo[info[2]]=newinfo; // update the mudinfo
    connected_muds[info[2]] = fd; // add this MUD to list of connected muds
    //trr("about to send the startup reply...","white");
    send_startup_reply(info[2]); // reply to MUD
    mudinfo_update_counter++;
    mudinfo_updates[info[2]]=mudinfo_update_counter;
    send_mudlist_updates(info[2], newinfo["old_mudlist_id"]);
    send_full_mudlist(info[2]);
    broadcast_mudlist(info[2]);
    broadcast_chanlist("foo",info[2]);
    if(bad_connects[newinfo["ip"]]) bad_connects[newinfo["ip"]] = 0;
    if(member_array("channel", keys(newinfo["services"])) != -1){
	//send_chanlist_reply(info[2], ( newinfo["old_chanlist_id"]) ? newinfo["old_chanlist_id"] : (random(1138) * 1138)  );

    }
    else {
	//trr("-------------------------------","blue");
	trr("It looks like "+info[2]+" doesn't have a channel service?!?","blue");
	//trr("These are the services reported: "+identify(newinfo["services"]),"blue");
	//trr("This is what newinfo looks like: "+identify(newinfo),"blue");
	//trr("-------------------------------","blue");
    }
    trr(timestamp()+" process_startup_req: for mud: "+info[2]+" complete.\n---\n","blue");
}
