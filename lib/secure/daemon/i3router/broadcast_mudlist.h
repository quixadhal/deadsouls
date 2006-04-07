// This file written completely by Tim Johnson (Tim@TimMUD)

// broadcast the mudlist when a mud just now got changed...
static void broadcast_mudlist(string mudname){
    string targ_mudname;
    mixed *packet1; // for muds that use protocol 1 or 2
    mixed *packet3; // for muds that use protocol 3
    Debug("broadcasting mudlist to let them know about "+mudname);
    packet1 = ({ "mudlist", 5, router_name, 0, "dummy", 0, mudinfo_update_counter,([
	mudname:({
	  connected_muds[mudname] ? -1 : 0,
	  // -1=up, 0=down, n=down for n secs
	  mudinfo[mudname]["ip"], // ip_addr
	  mudinfo[mudname]["player_port"], // player_port
	  mudinfo[mudname]["imud_tcp_port"], // imud_tcp_port
	  mudinfo[mudname]["imud_udp_port"], // imud_udp_port
	  mudinfo[mudname]["mudlib"], // mudlib
	  mudinfo[mudname]["base_mudlib"], // base_mudlib
	  mudinfo[mudname]["driver"], // driver
	  mudinfo[mudname]["mud_type"], // mud_type
	  mudinfo[mudname]["open_status"], // open_status
	  mudinfo[mudname]["services"], // services
	})
      ]) });
    packet3 = ({ "mudlist", 5, router_name, 0, "dummy", 0, mudinfo_update_counter,([
	mudname:({
	  (connected_muds[mudname] ? -1 : 0),
	  // -1=up, 0=down, n=down for n secs
	  mudinfo[mudname]["ip"], // ip_addr
	  mudinfo[mudname]["player_port"], // player_port
	  mudinfo[mudname]["imud_tcp_port"], // imud_tcp_port
	  mudinfo[mudname]["imud_udp_port"], // imud_udp_port
	  mudinfo[mudname]["mudlib"], // mudlib
	  mudinfo[mudname]["base_mudlib"], // base_mudlib
	  mudinfo[mudname]["driver"], // driver
	  mudinfo[mudname]["mud_type"], // mud_type
	  mudinfo[mudname]["open_status"], // open_status
	  mudinfo[mudname]["admin_email"], // admin_email
	  mudinfo[mudname]["services"], // services
	  mudinfo[mudname]["other_data"], // other_data
	})
      ]) });
    if(!mudinfo[mudname]){ // deleted mud...
	// just send a 0 to everyone
	foreach(targ_mudname in keys(connected_muds)){
	    write_data(connected_muds[targ_mudname], ({
		"mudlist", 5, router_name, 0, targ_mudname,
		0, mudinfo_update_counter, ([mudname:0])
	      }) );
	}
	return;
    }
    foreach(targ_mudname in keys(connected_muds)){
	switch(mudinfo[targ_mudname]["protocol"]){
	case 1:
	case 2:
	    packet1[4]=targ_mudname;
	    write_data(connected_muds[targ_mudname], packet1);
	    break;
	case 3:
	    packet3[4]=targ_mudname;
	    write_data(connected_muds[targ_mudname], packet3);
	    break;
	}
    }
}
