// This file written completely by Tim Johnson (Tim@TimMUD)

static void process_startup_req(int protocol, mixed info, int fd){
    // Handles startup stuff.
    //  Loads info into newinfo mapping.
    //  Error if mud is already connected.
    mapping newinfo;
    string site_ip, junk;
    // router name is info[4], I'll just pretend I'm every router though, ha-ha!
    // also, should verify that all the fields are the right type
    tn("info: "+identify(info));
    tn("process_startup_req: protocol="+protocol+", mud="+info[2],"green");
    if(sizeof(info)<18){ 
	// smallest protocol is protocol 1/2 which have size 18
	tn("THIS SHOULDNT BE HERE");
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
    //sscanf(socket_address(fd), "%s %s", site_ip,junk);
    //socket_bind(fd,random(1000)+10000);
    //site_ip = path_prefix(socket_status(fd)[4],".");
    //if(!site_ip || site_ip == "" || site_ip == "*"){
    //if(true()){
    tn("bad ip "+site_ip+" from "+info[2]);
    //socket_close(fd);
    //return;
    //}
    //junk = "foo";
    tn("fd is:" +fd);

    site_ip=socket_address(fd);
    tn("site_ip: "+site_ip,"red");
    newinfo = ([
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
    ]);
    tn("newinfo: "+identify(newinfo),"yellow");
    switch(protocol){
    case 1:
    case 2:
	if(sizeof(info)!=18){
	    tn("error","red");
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
	    tn("error","blue");

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
	tn("error","green");

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
	tn("mud already connected","cyan");
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
    if(mudinfo[info[2]] && mudinfo[info[2]]["password"] != newinfo["password"]){
	// if MUD is already known, not connected, and wrong password
	if(newinfo["ip"]==mudinfo[info[2]]["ip"]){
	    // same IP as last time... let's just trust 'em...
	    write_data(fd,({
		"error",5,router_name,0,info[2],0,
		"warning", // nothing in error summary that seems applicable?
		// Change later if I find a better error code
		"wrong password, but I'll allow you since it's the same IP as last time",0
	      }));
	}
	else{
	    tn("wrong password, and from a new IP","yellow");
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
    if(!mudinfo[info[2]]){
	// if new MUD, assign it a password
	newinfo["password"]=random(10000);
	// Change this maybe... see if the password is supposed to be in a certain range
    }
    // MUD should be okay at this point.
    Debug("about to update the mudinfo...");
    mudinfo[info[2]]=newinfo; // update the mudinfo
    connected_muds[info[2]] = fd; // add this MUD to list of connected muds
    Debug("about to send the startup reply...");
    send_startup_reply(info[2]); // reply to MUD
    mudinfo_update_counter++;
    mudinfo_updates[info[2]]=mudinfo_update_counter;
    send_mudlist_updates(info[2], newinfo["old_mudlist_id"]);
    broadcast_mudlist(info[2]);
    if(member_array("channel", keys(newinfo["services"])))
	send_chanlist_reply(info[2], newinfo["old_chanlist_id"]);
}
