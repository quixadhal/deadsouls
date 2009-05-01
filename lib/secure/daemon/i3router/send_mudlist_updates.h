// This file written mostly by Tim Johnson (Tim@TimMUD)

void send_mudlist_updates(string updating_mudname, int old_mudlist_id){
    string mudname; // mud name
    string *kys; // array of strings for keys that need to be sent
    mapping out=([ ]); // mapping to send
    int i; // counter
    int status;
    mapping payload = ([]);
    mudinfo_update_counter++;
    //trr("old_mudlist_id: "+old_mudlist_id,"blue");
    //trr("updating_mudname: "+updating_mudname,"blue");
    //trr("send_mudlist_updates, updating_mudname="+updating_mudname+", old_mudlist_id="+old_mudlist_id,"blue");
    foreach(mudname in keys(mudinfo_updates)){
        //trr("mudinfo_updates current element: "+identify(mudname));
#ifndef SEND_WHOLE_MUDLIST
        //trr("send whole mudlist undefined");
        if(mudinfo_updates[mudname]>old_mudlist_id){ // needs updating
#endif
            if(!mudinfo[mudname]){ // deleted mud;
                out[mudname] = 0;
            }
            else{
                if(undefinedp(connected_muds[mudname])){
                    if(mudinfo[mudname]["disconnect_time"] &&
                            mudinfo[mudname]["restart_delay"] == -1) status = 0;
                    else  status = mudinfo[mudname]["restart_delay"];
                }
                else status = -1;
                //trr("STATUS: "+status,"yellow");
                switch(mudinfo[updating_mudname]["protocol"]){
                    case 1:
                    case 2:
                        out[mudname] = ({
                                status,
                                // -1=up, 0=down, n=down for n secs
                                mudinfo[mudname]["restart_delay"],
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
                                });
                        break;
                    case 3:
                        out[mudname] = ({
                                status,
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
                                });
                        break;
                } // switch
            } // else not deleted
#ifndef SEND_WHOLE_MUDLIST
        } // if needs updating
#endif
    } // foreach
    // out now has the mapping of what to tell the mud about...
    // this might be big, so later I'll make it so it sends only 10 muds at a time...
    // take an idea from /cmds/std/_semote.c
    kys=keys(out);
    for(i=0;i<sizeof(kys)-10;i+=10){ // leave a group of size between 1 and 10 remaining
        //trr("sending a set of 10...");
        // send all the ones except the last one, doing 10 at a time,
        // but give them their original mudlist id, in case they get
        // disconnected before all the sets of 10 get finished
        payload = ([  kys[i]: out[kys[i]],
                kys[i+1]: out[kys[i+1]],
                kys[i+2]: out[kys[i+2]],
                kys[i+3]: out[kys[i+3]],
                kys[i+4]: out[kys[i+4]],
                kys[i+5]: out[kys[i+5]],
                kys[i+6]: out[kys[i+6]],
                kys[i+7]: out[kys[i+7]],
                kys[i+8]: out[kys[i+8]],
                kys[i+9]: out[kys[i+9]],
                ]);
        write_data(connected_muds[updating_mudname],({
                    "mudlist",
                    5,
                    router_name,
                    0,
                    updating_mudname,
                    0,
                    //old_mudlist_id,
                    (random(9999) * random(999)),
                    payload,
                    }));
        //this_object()->SendList(payload);
        map_delete(out,kys[i]);
        map_delete(out,kys[i+1]);
        map_delete(out,kys[i+2]);
        map_delete(out,kys[i+3]);
        map_delete(out,kys[i+4]);
        map_delete(out,kys[i+5]);
        map_delete(out,kys[i+6]);
        map_delete(out,kys[i+7]);
        map_delete(out,kys[i+8]);
        map_delete(out,kys[i+9]);
    }
    // incomplete set is left where it kicked out of the for loop
    write_data(connected_muds[updating_mudname],({
                "mudlist",
                5,
                router_name,
                0,
                updating_mudname,
                0,
                mudinfo_update_counter,
                out,
                }));
    //this_object()->SendList(out);
    mudinfo[updating_mudname]["old_mudlist_id"]=mudinfo_update_counter;
}
