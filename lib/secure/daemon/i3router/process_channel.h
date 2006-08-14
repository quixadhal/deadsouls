// This file written completely by Tim Johnson (Tim@TimMUD)
#include <save.h>

static void process_channel(int fd, mixed *info){
    string mudname;
    string sendermsg, sendername, senderrealname, sendermud;
    string targetname, targetmud, targstr = "";
    trr("extra stuff is ["+info[0][8..]+"]");
    switch(info[0][8..]){ // what is after the "channel-"
    case "m": // message
    case "e": // emote
    case "t": // targetted emote
	trr("they want to do a message...");
	// (drop-through from m/e is intentional)
	// Probably should check if the target is 0@0 like it should be,
	// and give a warning if it's not... I don't carethough, I'll just
	// ignore the target and save some CPU.
	// Check if string parts are strings...
	if(info[0][8..]=="t"){
	    if(sizeof(info)!=13 || !stringp(info[9]) || !stringp(info[10]) ||
	      !stringp(info[11]) || !stringp(info[12])){
		send_error(info[2],info[3],"bad-pkt","Bad packet format.",info);
		return;
	    }
	    if(!grepp(info[9],"\$N")){
		info[9] = info[9] + " (from "+info[3]+"@"+info[2]+")";
		info[10] = info[10] + " (from "+info[3]+"@"+info[2]+")";
	    }
	    sendername = info[11];
	    senderrealname = info[3];
	    sendermud = info[2];
	    targetname = info[12];
	    targetmud = info[7];
	    sendermsg = info[9];
	}
	else{ // m, e
	    if(sizeof(info)!=9){
		send_error(info[2],info[3],"bad-pkt","Bad packet format.",info);
		return;
	    }
	    if(info[0] == "channel-e" && !grepp(info[8],"$N"))
		info[8] = info[8] + " (from "+info[3]+"@"+info[2]+")";
	    sendername = info[7];
	    senderrealname = info[3];
	    sendermud = info[2];
	    sendermsg = info[8];
	}
	if(targetname && targetmud) {
	    targstr = "Target: "+targetname+"@"+targetmud;
	}

	if(info[4]!=0 || info[5]!=0 || !stringp(info[6]) ||
	  !stringp(info[7]) || !stringp(info[8])){
	    send_error(info[2],info[3],"bad-pkt","Bad packet format.",info);
	    return;
	}
	if(!channels[info[6]]){ // error, channel is not registered!
	    send_error(info[2],info[3],"unk-channel",
	      "Unknown channel: "+info[6],info);
	    return;
	}
	if(member_array(info[2],listening[info[6]])==-1){
	    // if mud is not listening, then make them listen
	    // and let them know they weren't listening.
	    send_error(info[2],info[3],"warning",
	      // No applicable message codes... change
	      // "warning" to something else if you want.
	      "Your mud wasn't listening to "+info[6]+
	      ", so I'll start you listening right now.",info);
	    // Could do all the listen stuff, or could just
	    // pretend they requested it... I'm gonna pretend...
	    process_channel(fd,({ "channel-listen", 5, info[2], 0,
		router_name, 0, info[6], 1 }));
	    return;
	}
	if(channels[info[6]][0]==2){ // filtered channel
	    if(channels[info[6]][1]!=info[2]){ // not from chan owner
		// if from anybody else other than the owner,
		// check if the owner is online, then either give
		// an error, or forward to the owner...
		if(!connected_muds[channels[info[6]][1]]){
		    send_error(info[2],info[3],"unk-dst",
		      channels[info[6]][1]+" is not online to filter "+info[6],
		      info);
		    return;
		}
		if(info[4]!=0 || info[5]!=0 || !stringp(info[6]) ||
		  !stringp(info[7]) || !stringp(info[8])){
		    send_error(info[2],info[3],"bad-pkt","Bad packet format",info);
		    return;
		}
		write_data(connected_muds[channels[info[6]][1]],({
		    "chan-filter-req",
		    5,
		    router_name,
		    0,
		    channels[info[6]][1], // mud name of chan owner
		    0,
		    info[6],
		    info
		  }));
		return;
	    }
	    // IS from chan owner, just broadcast it...
	    // drop through and broadcast like the other types do...
	}
	trr("CHAN: I think it's a good message at this point...");
	// at this point, they're wanting to do a message on a
	// selective banned/allowed channel, or else are the owner
	// of a filtered channel and they have not been
	// blocked, so I will just do it...

	log_file("router/"+info[6], timestamp()+" "+
	  sendername+"("+senderrealname+")@"+sendermud+" "+": "+
	  sendermsg+" "+targstr+"\n");

	foreach(mudname in keys(connected_muds)){
	    if(member_array(mudname, listening[info[6]])!=-1)
		write_data(connected_muds[mudname],info);
	}

	//broadcast_data(muds_not_on_this_fd(fd), info);
	// send to all the other fd's that have a mud listening
	//			write_data(fd,info);
	// also send the message to the connection that this mud is on
	// from too... why do this? 'cause I'm allowing multiple muds to
	// log on from the same connection :)
	// I think the normal I3 server does this too, but I don't know
	// since I can't test at the moment.
	return;
    case "add":
	// check if already exists...
	if(channels[info[6]]){
	    trr(info[3]+"@"+info[2]+" failed to create the channel: "+info[6]+
	      "because it already exists.","red");
	    return;
	}
	// check if a valid channel name (illegal characters?)
	// check if other stuff is valid, like channel_type is 0,1,2
	// at this point, is being successfully added...
	channel_update_counter++;
	channels[info[6]]=({ info[7], info[2], ({}) });
	channel_updates[info[6]] = channel_update_counter;
	trr(info[3]+"@"+info[2]+" created the channel: "+info[6],"yellow");
	// broadcast an update saying that this channel is added or changed now
	// chanlist-reply packet to everybody (who has a channel service?)
	broadcast_chanlist(info[6]);
	//			broadcast_data(muds_not_on_this_fd(fd), ({
	//				"chanlist-reply",5,router_name,0,
	//				0, // should be mudname, I think 0 would work just as well?
	//				0, channel_update_counter,
	//				([ info[6]:({ info[2], info[7] }) ])
	//			}));
	//			write_data(fd,({
	//				"chanlist-reply",5,router_name,0,
	//				0, // should be mudname, I think 0 would work just as well?
	//				0, channel_update_counter,
	//				([ info[6]:({ info[2], info[7] }) ])
	//			}));
	save_object(SAVE_ROUTER);
	return;
    case "remove":
	if(!channels[info[6]]){ // error, channel is not registered!
	    send_error(info[2],info[3],"unk-channel",
	      "Unknown channel: "+info[6],info);
	    return;
	}
	if(channels[info[6]][1]!=info[2]){ // error, not owner!
	    send_error(info[2],info[3],"not-allowed","Channel "+
	      info[6]+" owned by: "+channels[info[6]][1],info);
	    return;
	}
	// at this point, is being removed by the owner...
	channel_update_counter++;
	map_delete(channels,info[6]);
	channel_updates[info[6]] = channel_update_counter;
	trr(info[3]+"@"+info[2]+" deleted the channel: "+info[6],"yellow");
	// broadcast an update saying that this channel is gone now
	save_object(SAVE_ROUTER);
	return;
    case "admin":
	// add/delete muds from the 2 lists...
	if(!listening[info[6]]) listening[info[6]] = ({});
	if(info[7]){ // add to list...
	    channels[info[6]][2] += info[7];
	    // if add to ban list, unlisten...
	    if(channels[info[6]][0]==0) // type 0 means selective ban
		listening[info[6]] -= info[7];
	}
	if(info[8]){ // remove from list...
	    channels[info[6]][2] += info[8];
	    if(channels[info[6]][0]!=0) // type 0 means selective ban...
		// selective allow and filtered are the same though...
		// so if not selective ban, then act like selective allow...
		// if removed from allow list, unlisten...
		listening[info[6]] -= info[8];
	}
	save_object(SAVE_ROUTER);
	return;
    case "listen": // mudname=info[2], channame=info[6], on_or_off=info[7]
	if(!channels[info[6]]){ // error, channel is not registered!
	    send_error(info[2],info[3],"unk-channel",
	      "Unknown channel: "+info[6],info);
	    return;
	}
	if(!listening) listening=([ ]);
	if(!listening[info[6]]) listening[info[6]]=({ }); // first MUD to listen
	if(info[7]){ // on_or_off is on
	    if(member_array(info[2],listening[info[6]])!=-1)
		return; // already listening, ignore them
	}
	else{ // on_or_off is on
	    if(member_array(info[2],listening[info[6]])==-1)
		return; // already NOT listening, ignore them
	}
	trr("listening change on chan:"+info[6]+", mud="+info[2]+", on_or_off="+info[7]);
	// only CHANGES should get to this point
	switch(channels[info[6]][0]){
	case 0: // selectively banned
	    if(member_array(info[2],channels[info[6]][2])!=-1){
		// in list, you're banned...
		send_error(info[2],0,"not-allowed",
		  "Banned from "+info[6],info);
		save_object(SAVE_ROUTER);
		return;
	    }
	    // not in ban list at this point
	    if(info[7])
		listening[info[6]] += ({ info[2] });
	    else
		listening[info[6]] -= ({ info[2] });
	    save_object(SAVE_ROUTER);
	    return;
	case 1: // selectively allowed
	    if(member_array(info[2],channels[info[6]][2])==-1 &&
	      info[2] != channels[info[6]][1]){
		// NOT in list, you're banned!
		send_error(info[2],0,"not-allowed",
		  "Not in allow list for "+info[6],info);
		return;
	    }
	    // in allow list at this point
	    if(info[7])
		listening[info[6]] += ({ info[2] });
	    else
		listening[info[6]] -= ({ info[2] });
	    save_object(SAVE_ROUTER);
	    return;
	case 2: // filtered... act like selectively allowed
	    if(member_array(info[2],channels[info[6]][2])==-1 &&
	      info[2] != channels[info[6]][1]){
		// NOT in list, you're banned!
		send_error(info[2],0,"not-allowed",
		  "Not in allow list for "+info[6],
		  info);
		return;
	    }
	    // in allow list at this point
	    if(info[7])
		listening[info[6]] += ({ info[2] });
	    else
		listening[info[6]] -= ({ info[2] });
	    save_object(SAVE_ROUTER);
	    return;
	} // switch
    default: // trying to do "channel-blah"
	send_error(info[2],info[3],"unk-type","I don't know what "+info[0]+
	  " means.",info);
	trr("Don't know what the ["+info[0]+"] packet means.", "yellow");
	return;
    }
    trr("can't get here?");
}
