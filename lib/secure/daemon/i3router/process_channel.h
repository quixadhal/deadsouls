// This file written mostly by Tim Johnson (Tim@TimMUD)
#include <save.h>
#include <daemons.h>

#ifndef FILTER_D
#define FILTER_D "/secure/daemon/filter"
#endif

static void process_channel(mixed fd, mixed *info){
    string mudname;
    string sendermsg, sendername, senderrealname, sendermud;
    string targetname, targetmud, targstr = "";
    switch(info[0][8..]){ // what is after the "channel-"
        case "m": // message
            case "e": // emote
            case "t": // targetted emote
            // (drop-through from m/e is intentional)
            // Probably should check if the target is 0@0 like it should be,
            // and give a warning if it's not... I don't carethough, I'll just
            // ignore the target and save some CPU.
            // Check if string parts are strings...
            if(info[0][8..]=="t"){
                if(sizeof(info)!=13 || !stringp(info[9]) || !stringp(info[10]) ||
                        !stringp(info[11]) || !stringp(info[12]) ||!sizeof(info[11])){
                    send_error(info[2],info[3],"bad-pkt","Bad packet format.",info);
                    return;
                }
                if(!grepp(info[9],"$N")){
                    info[9] = info[9] + " (from "+info[11]+"@"+info[2]+")";
                    info[10] = info[10] + " (from "+info[11]+"@"+info[2]+")";
                }
                sendername = info[11];
                senderrealname = info[3];
                sendermud = info[2];
                targetname = info[12];
                targetmud = info[7];
                sendermsg = info[9];
            }
            else{ // m, e
                if(sizeof(info)!=9 || !stringp(info[3]) || !sizeof(info[3])){
                    send_error(info[2],info[3],"bad-pkt","Bad packet format.",info);
                    return;
                }
                if(!stringp(info[7]) || !sizeof(info[7])){
                    info[7] = info[3];
                }
                if(info[0] == "channel-e" && !grepp(info[8],"$N"))
                    info[8] = info[8] + " (from "+info[7]+"@"+info[2]+")";
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
        if(listening[info[6]] &&
                member_array(info[2],listening[info[6]])==-1){
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
        }
        if(channels[info[6]][0]==2){ // filtered channel
            if(channels[info[6]][1]!=info[2]){ // not from chan owner
                // if from anybody else other than the owner,
                // check if the owner is online, then either give
                // an error, or forward to the owner...
                if(undefinedp(connected_muds[channels[info[6]][1]])){
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
        // at this point, they're wanting to do a message on a
        // selective banned/allowed channel, or else are the owner
        // of a filtered channel and they have not been
        // blocked, so I will just do it...

        server_log(sendername+"("+senderrealname+")@"+sendermud+" "+": "+
                sendermsg+" "+targstr+"\n",info[6]);

        if(member_array(info[2],channels[info[6]][2])!=-1){
            // in list, you're banned...
            send_error(info[2],0,"not-allowed",
                    "Banned from "+info[6],info);
            return;
        }

        if(intp(fd)) this_object()->SendMessage(info);

        else if(member_array(fd, this_object()->GetBannedMuds()) != -1){
            return;
        }

        info[8] = FILTER_D->eventFilter(info[8]);

        foreach(mudname in keys(connected_muds)){
            if(listening[info[6]] && member_array(mudname, listening[info[6]])!=-1)
                write_data(connected_muds[mudname],info);
        }
        return;
        case "add":
            if(!stringp(info[4])) info[4] = this_object()->GetRouterName();
        // check if already exists...
        if(!info[6]) return;
        if(channels[info[6]]){
            send_error(info[2],info[3],"not-allowed",
                    "Channel already exists: "+info[6],info);
            trr(info[3]+"@"+info[2]+" failed to create the channel: "
                    +info[6]+ " because it already exists.","red");
            return;
        }
        // check if a valid channel name (illegal characters?
        if(!stringp(info[6]) || sizeof(info[6]) > 80){
            send_error(info[2],info[3],"not-allowed",
                    "Invalid channel name: "+identify(info[6]),info);
            trr(info[3]+"@"+info[2]+" failed to create the channel: "
                    +info[6]+ "because it has an invalid name.","red");
            return;
        } 
        // check if other stuff is valid, like channel_type is 0,1,2
        if(!intp(info[7]) || (info[7] < 0 || info[7] > 2)){
            send_error(info[2],info[3],"not-allowed",
                    "Invalid channel type: "+identify(info[7]),info);
            trr(info[3]+"@"+info[2]+" failed to create the channel: "
                    +info[6]+ "because it has an invalid type.","red");
            return;
        }
        // at this point, is being successfully added...
        channel_update_counter++;
        channels[info[6]]=({ distinct_array(info[7]), info[2], ({}) });
        channel_updates[info[6]] = channel_update_counter;
        server_log(info[3]+"@"+info[2]+" created the channel: "+info[6]+"\n");
        // broadcast an update saying that this channel is added or changed now
        // chanlist-reply packet to everybody (who has a channel service?)
        broadcast_chanlist(info[6]);
        SendList( ([ "channels" : ([ info[6] : channels[info[6]] ]), 
                "listening" : ([]) ]),
                0, "chanlist" );
        SaveObject(SAVE_ROUTER);
        return;
        case "remove":
            if(!stringp(info[4])) info[4] = this_object()->GetRouterName();
        if(!channels[info[6]]){ // error, channel is not registered!
            send_error(info[2],info[3],"unk-channel",
                    "Unknown channel: "+info[6],info);
            return;
        }
        if(channels[info[6]][1] != info[2] && 
                clean_fd(socket_address(fd)) != router_ip ){
            send_error(info[2],info[3],"not-allowed","Channel "+
                    info[6]+" owned by: "+channels[info[6]][1],info);
            return;
        }
        // at this point, is being removed by the owner...
        channel_update_counter++;
        map_delete(channels,info[6]);
        map_delete(channel_updates,info[6]);
        server_log(info[3]+"@"+info[2]+" deleted the channel: "+info[6]+"\n");
        // broadcast an update saying that this channel is gone now
        broadcast_chanlist(info[6]);
        SaveObject(SAVE_ROUTER);
        SendList( ([ "channels" : ([ info[6] : -1 ]), "listening" : ([]) ]),
                0, "chanlist" );
        return;
        case "admin":
        if(!stringp(info[4])) info[4] = this_object()->GetRouterName();
        // add/delete muds from the 2 lists...
        if(channels[info[6]][1]!=info[2] && 
                clean_fd(socket_address(fd)) != router_ip ){
            send_error(info[2],info[3],"not-allowed","Channel "+
                    info[6]+" owned by: "+channels[info[6]][1],info);
            return;
        }
        if(!listening[info[6]]) listening[info[6]] = ({});
        if(sizeof(info[7])){ // add to list...
            channels[info[6]][2] += info[7];
            channels[info[6]][2] = distinct_array(channels[info[6]][2]);
            // if add to ban list, unlisten...
            if(channels[info[6]][0]==0){ // type 0 means selective ban
                listening[info[6]] -= info[7];
                listening[info[6]] = distinct_array(listening[info[6]]);
            }
        }
        if(sizeof(info[8])){ // remove from list...
            channels[info[6]][2] -= info[8];
            channels[info[6]][2] = distinct_array(channels[info[6]][2]);
            if(channels[info[6]][0]!=0){ // type 0 means selective ban...
                // selective allow and filtered are the same though...
                // so if not selective ban, then act like selective allow...
                // if removed from allow list, unlisten...
                listening[info[6]] -= info[8];
                listening[info[6]] = distinct_array(listening[info[6]]);
            }
        }
        SendList( ([ "channels" : ([ info[6] : channels[info[6]] ]) ]),
                    0, "chanlist" );
        SaveObject(SAVE_ROUTER);
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
        else{ // on_or_off is off
            if(member_array(info[2],listening[info[6]])==-1)
                return; // already NOT listening, ignore them
        }
        // only CHANGES should get to this point
        switch(channels[info[6]][0]){
            case 0: // selectively banned
                if(member_array(info[2],channels[info[6]][2])!=-1){
                    // in list, you're banned...
                    send_error(info[2],0,"not-allowed",
                            "Banned from "+info[6],info);
                    return;
                }
                // not in ban list at this point
                if(info[7])
                    listening[info[6]] += ({ info[2] });
                else
                    listening[info[6]] -= ({ info[2] });
                listening[info[6]] = distinct_array(listening[info[6]]);
                SaveObject(SAVE_ROUTER);
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
                listening[info[6]] = distinct_array(listening[info[6]]);
                SaveObject(SAVE_ROUTER);
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
                listening[info[6]] = distinct_array(listening[info[6]]);
                SaveObject(SAVE_ROUTER);
                return;
        } // switch
        default: // trying to do "channel-blah"
        send_error(info[2],info[3],"unk-type","I don't know what "+info[0]+
                " means.",info);
        return;
    }
}

void list_chans(){
    mapping tmp_chans = channels;
    foreach(mixed key, mixed val in tmp_chans){
        trr("Key: "+identify(key));
        trr("Val: "+identify(val));
        trr("--\n","red");
    }
}

void AddIMC2Chan(mixed fd, string name, string chan){
    if(base_name(previous_object()) != IMC2_SERVER_D) return;
    process_channel(fd,({"channel-listen",5,name,0,router_name,0,chan,1}));
}
