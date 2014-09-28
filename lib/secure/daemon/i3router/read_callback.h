string *blacklisted_muds = ({});
static string *graylisted_muds = ({});
static mapping packet_counter = ([]); 

void setup_blacklist(){
    if(file_exists(ROUTER_BLACKLIST)){
        blacklisted_muds += read_big_file(ROUTER_BLACKLIST);
        blacklisted_muds = singular_array(blacklisted_muds);
        //tc(identify(sizeof(blacklisted_muds)),"red");
    }
}

void read_callback(mixed fd, mixed info){
    mixed tmp;
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
    string *logspam = ({ "auth-mud-req", "auth-mud-reply", "tell", "emoteto",
            "ping","channel-listen", "ping-req", "ping-reply" });
    string ip_addr = (stringp(fd) ? fd : clean_fd(socket_address(fd)));

    if(!packet_counter) packet_counter = ([]);

    if(!strsrch(info[0],"irn-")){
        irn_read_callback(fd, info);
        return;
    }

    if(info[4] && !grepp(info[0],"startup-req") && 
            member_array(info[4], keys(connected_muds)) == -1 &&
            member_array(info[2], keys(connected_muds)) == -1){
        return;
    }
    validate();
    if(sizeof(blacklisted_muds)){
        if(member_array(info[2], blacklisted_muds) != -1) return;
        if(intp(fd)){
            if(member_array(ip_addr, blacklisted_muds) != -1) return;
            foreach(string bmud in blacklisted_muds){
                if(sscanf(bmud, "%*d.%*s") && !strsrch(ip_addr, bmud)){
                    return;
                }
            }
        }
    }
    if(!packet_counter[info[2]]){
        packet_counter[info[2]] = ([ "time" : time(), "count" : 0 ]);
    }
    if(packet_counter[info[2]]["time"] != time()){
        packet_counter[info[2]]["count"] = 0;
        packet_counter[info[2]]["time"] = time();
    }
    if(info[0] != "channel-listen"){
        if(!strsrch(info[0], "startup-req")){
            packet_counter[info[2]]["count"] += 6;
        }
        else packet_counter[info[2]]["count"]++;
    }
    if(sizeof(graylisted_muds)){
        if((info[2] && member_array(info[2], graylisted_muds) != -1) ||
                (intp(fd) && member_array(ip_addr, graylisted_muds) != -1)){
            if(info[2] && member_array(info[2], keys(connected_muds)) != -1){
                if(packet_counter[info[2]]["count"] < 2){
                    send_error(info[2],info[3],"not-allowed",
                        "Your packets are being temporarily dropped "+
                        "due to flooding.", info);
                }
            }
            return;
        }
    }

    if(packet_counter[info[2]]["count"] > 10 &&
            member_array(fd, keys(ROUTER_D->query_irn_sockets())) == -1){
        if(info[2] && member_array(info[2], keys(connected_muds)) != -1){
            send_error(info[2],info[3],"not-allowed",
                    "Flood detected! Your packets will be temporarily ignored.",
                    info);
        }
        graylisted_muds += ({ info[2] });
        trr("Graylisting "+identify(info[2])+" from "+ip_addr,"red");
        return;
    }
    if(member_array(info[0],logspam) == -1 && strsrch(info[0],"irn-")){
        string foo = "IRN";
        if(intp(fd)) foo = socket_address(fd);
        trr(timestamp()+" Received from fd("+fd+"), source: "+foo+"\n"+identify(info), "green");
    }
    // Base info in a packet is of size 6.
    // comment out the statements below to avoid a big channel log file
    if(grepp(info[0],"chan") && !grepp(info[0],"channel-listen")) 
        server_log(identify(info),"chan_log");

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
    if(info[4]!=0 && !mudinfo[info[4]] && info[4]!=router_name){
        // if target mud is not 0 (broadcasting), not the router name,
        // and not a known MUD
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
        trr("Error [unk-dst], because target is "+info[4]+" and thus invalid.");
        return;
    }
    if(sscanf(info[0],"startup-req-%d",i)==1){
        // special condition for startup-req...
        trr("calling process_startup_req, i="+i+", fd="+fd+" which is: "+socket_address(fd));
        //call_other(this_object(),"process_startup_req",i,info,fd);
        this_object()->process_startup_req(i,info,fd);
        return;
    }
    if(intp(fd) && !undefinedp(connected_muds[info[2]]) && (connected_muds[info[2]]!=fd)){
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
        server_log("They have not done a startup-req for fd="+fd+", mudname="+info[2]);
        return;
    }
    // at this point, I guess it has a valid origin and stuff
    if(sscanf(info[0],"channel-%*s")==1){ // command has a "channel-" prefix
        // special case for channel stuff
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
        if(member_array(info[7][6],({ "channel-a", "channel-e", "channel-t"}))==-1){
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
        if(intp(fd)) this_object()->SendMessage(info);
        foreach(mudname in keys(connected_muds)){
            if(member_array(mudname, listening[info[6]])!=-1)
                write_data(connected_muds[mudname],info[7]);
        }
        return;
    }
    if(info[4]==0){ // if broadcasting this...
        if(intp(fd)) {
            this_object()->SendMessage(info);
        }
        broadcast_data(connected_muds,info);
        return;   
    }
    if(info[4]==router_name) {
        switch(info[0]){
            case "shutdown" :
                if(sizeof(info) != 7){
                    send_error(info[2],info[3],"bad-pkt","Wrong number of elements for packet: "+info[0],info);
                    return;
                }
            if(info[6] > 604800) {
                server_log("ROUTER_D: deleting "+this_object()->query_connected_fds()[fd]+" per "+
                        "request of "+info[2]+"\n");
                remove_mud(this_object()->query_connected_fds()[fd], 1);
            }
            else {
                server_log("ROUTER_D: disconnecting "+this_object()->query_connected_fds()[fd]+" per "+
                        "request of "+info[2]);
                this_object()->disconnect_mud(this_object()->query_connected_fds()[fd]);
            }
            break;
            case "chanlist-req" : case "chanlist-request" :
                broadcast_chanlist("foo",info[2]);
            break;
            default :
            // Something meant for the router but not handled by now!
            send_error(info[2],info[3],"not-imp","Unknown command sent to router: "+info[0],info);
            server_log("%^RED%^UNHANDLED PACKET:\n"+identify(info));
        }
        return;
    }
    if(info[0]=="startup-reply" || info[0]=="chan-filter-reply"||
            info[0]=="chanlist-reply" || info[0]=="mudlist" ||
            info[0]=="bad-mojo"){
        // Thanks to Tricky for pointing out the need for this safeguard
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
        }	if(sizeof(info)==10 && mudinfo[info[4]]["protocol"]<=2){
            // target mud is being sent a protocol 3 response,
            // but only understands 1 & 2, so strip the extra info
            write_data(connected_muds[info[4]], ({
                        info[0],info[1],info[2],info[3],info[4],info[5],
                        info[6],info[7]
                        }));
            return;
        }
    }
    // if fd is an integer (meaning it's locally connected) and info[4] is zero
    // (meaning it's a boradcast) then we send data to irn. Also, if fd is an
    // integer, info[4] is non-zero, and info[4] is not a connected mud.
    if((!info[4] && intp(fd)) || 
            (intp(fd) && info[4] && undefinedp(connected_muds[info[4]])) ){
        this_object()->SendMessage(info);
    }
    tmp = (this_object()->query_connected_muds()[info[4]] || -1);
    if(tmp > -1){
        write_data(connected_muds[info[4]], info);
    }
    return;
}
