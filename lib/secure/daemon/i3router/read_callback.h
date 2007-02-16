static string *blacklisted = explode(read_file(ROUTER_BLACKLIST),"\n");

void read_callback(int fd, mixed info){
    // This is called when messages come in from a MUD.
    // Should reject all messages if they have not done a (successful) startup-req,
    // Should check to make sure the fd matches with the mud they are claiming to be, else error.
    // because all packets require an originator_mudname.

    // If target is the name of the router, then call the function in this object.
    // if target is 0, then broadcast to muds.
    // Do an error if target mudname is not known.
    // 

    // According to: http://www.intermud.org/i3/specs/formats.php3
    // Transmissions are LPC arrays with a predefined set of six initial elements:
    // ({ type, ttl, originator mudname, originator username, target mudname, target username, ... }). 
    // info[0]=type
    // info[1]=ttl
    // info[2]=originator mudname
    // info[3]=originator username
    // info[4]=target mudname
    // info[5]=target username

    string mudname;
    int i;

    validate();

    if(sizeof(blacklisted)){
	if(member_array(info[2], blacklisted) != -1) return;
	if(member_array(clean_fd(socket_address(fd)), blacklisted) != -1) return;
    }

    //trr("Incoming data from fd("+fd+"), address "+socket_address(fd)+".");
    //trr("The known status of that fd is "+identify(socket_status(fd)));

    //trr("Received from fd("+fd+"), fd("+socket_address(fd)+")\n"+identify(info),((info[0] == "auth-mud-req" || info[0] == "auth-mud-reply") ? "magenta" : "green"));
    if(info[0] != "auth-mud-req" && info[0] != "auth-mud-reply" && info[0] != "channel-listen" &&
      info[0] != "channel-listen" && info[0] != "tell" && info[0] != "ping" &&
      info[0] != "emoteto")
	trr(timestamp()+" Received from fd("+fd+"), fd("+socket_address(fd)+")\n"+identify(info), "green");
    // Base info in a packet is of size 6.
    if(grepp(info[0],"chan") && !grepp(info[0],"channel-listen")) log_file("router/packet_log",identify(info)+"\n");
    if(sizeof(info)<6 ||
      !stringp(info[0]) ||
      !intp(info[1]) || !stringp(info[2]) ||
      (!stringp(info[3]) && info[3]!=0) ||
      (!stringp(info[4]) && info[4]!=0) ||
      (!stringp(info[5]) && info[5]!=0) ) {
	write_data(fd,({
	    // okay, their initial data isn't fully there...
	    // careful about array out of bounds...
	    "error",5,router_name,0,
	    (sizeof(info)>=3 ? info[2] : 0), // mud name
	    (sizeof(info)>=4 ? info[3] : 0), // user name
	    "bad-pkt","Invalid initial data",info
	  }));
	return;
    }
    //if(lower_case(info[4]) == "yatmin") info[4] = "yatmim";
    if(info[4]!=0 && !connected_muds[info[4]] && info[4]!=router_name){
	// if target mud is not 0 (broadcasting), not the router name,
	// and not a connected MUD
	write_data(fd,({
	    "error",
	    5,
	    router_name,
	    0,
	    info[2],
	    info[3],
	    "unk-dst", // same as I3
	    "destination unknown", // same as I3
	    info
	  }));
	//trr("Error [unk-dst], because target is "+info[4]+" and thus invalid.");
	return;
    }
    if(sscanf(info[0],"startup-req-%d",i)==1){
	// special condition for startup-req...
	//trr("calling process_startup_req, i="+i+", fd="+fd+" which is: "+socket_address(fd));
	//call_other(this_object(),"process_startup_req",i,info,fd);
	this_object()->process_startup_req(i,info,fd);
	return;
    }
    if(!connected_muds[info[2]] || (connected_muds[info[2]]!=fd)){
	// MUD hasn't done a startup-req yet
	write_data(fd,({
	    "error",
	    5,
	    router_name,
	    0, // originator username
	    info[2], // mud name
	    info[3], // user name
	    "unk-src", // error code
	    "Your MUD hasn't registered as "+info[2]+" yet", // Error message
	    info
	  }));
	//trr("They have not done a startup-req for fd="+fd+", mudname="+info[2]);
	log_file("router/server_log",timestamp()+" They have not done a startup-req for fd="+fd+", mudname="+info[2]+"\n");
	return;
    }
    // at this point, I guess it has a valid origin and stuff
    if(sscanf(info[0],"channel-%*s")==1){ // command has a "channel-" prefix
	// special case for channel stuff
	//trr("calling process_channel...");
	process_channel(fd,info);
	return;
    }
    if(info[0]=="chan-filter-reply"){
	if(!stringp(info[6]) || sizeof(info[7])<9 ){
	    // avoid out-of-bounds
	    send_error(info[2],info[3],"bad-pkt",
	      "Invalid chan-filter-reply packet.",info);
	}
	if(channels[info[6]][1]!=info[2]){
	    send_error(info[2],info[3],"not-allowed",
	      "You are not the owner of "+info[6],info);
	    return;
	}
	if(member_array(info[7][6],({ "channel-a", "channel-e", "channel-t"}))==-11){
	    // Not a valid channel packet.
	    send_error(info[2],info[3],"not-allowed","*giggles*",info);
	}
	if(info[6]!=info[7][6]){
	    // They're trying to trick me into broadcasting
	    // fake messages on another channel!
	    // That's pretty funny :P
	    // I'll have to remember to check if the official router falls for it.
	    send_error(info[2],info[3],"not-allowed","Hehe!",info);
	    return;
	}
	if(member_array(info[7][6],({ "channel-a", "channel-e", "channel-t"}))){

	}
	// Broadcast the message...
	foreach(mudname in keys(connected_muds)){
	    if(member_array(mudname, listening[info[6]])!=-1)
		write_data(connected_muds[mudname],info[7]);
	}
	return;
    }

    if(info[4]==0){ // if broadcasting this...
	broadcast_data(connected_muds,info);
	return;
    }
    if(info[4]==router_name) {
	// Something meant for the router but not handled by now!
	send_error(info[2],info[3],"not-imp","Unknown command sent to router: "+info[0],info);
	//trr("unhandled packet meant for router: "+info[0],"red");
	log_file("router/server_log","UNHANDLED PACKET:\n"+identify(info)+"\n");
	return;
    }
    if(info[0]=="startup-reply" || info[0]=="chan-filter-reply"||
      info[0]=="chanlist-reply" || info[0]=="mudlist" ||
      info[0]=="bad-mojo"){
	send_error(info[2],info[3],"not-allowed",
	  "You are not allowed to send this kind of packet.",info);
	return;
    }
    // at this point, I guess you should forward it to the destination...
    if(info[0]=="locate-reply"){
	// Special case for locate-reply, because protocol 3 has a larger packet...
	if(sizeof(info)==8 && mudinfo[info[4]]["protocol"]>2){
	    // originator mud is sending a protocol 1/2 response,
	    // but target understands 3, so add the extra info
	    write_data(connected_muds[info[4]], ({
		info[0],info[1],info[2],info[3],info[4],info[5],
		info[6],info[7],0,0
	      }));
	    return;
	}
	if(sizeof(info)==10 && mudinfo[info[4]]["protocol"]<=2){
	    // target mud is being sent a protocol 3 response,
	    // but only understands 1 & 2, so strip the extra info
	    write_data(connected_muds[info[4]], ({
		info[0],info[1],info[2],info[3],info[4],info[5],
		info[6],info[7]
	      }));
	    return;
	}
    }
    write_data(connected_muds[info[4]], info);
    return;
}
