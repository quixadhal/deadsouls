// This file written mostly by Tim Johnson (Tim@TimMUD)

// broadcast the mudlist when a mud just now got changed...

static void validate();

static mapping BroadcastQueue = ([]);

varargs void broadcast_mudlist(string mudname, int remote, string single){
    //string targ_mudname;
    string *audience = ({});
    mixed *packet1; // for muds that use protocol 1 or 2
    mixed *packet3; // for muds that use protocol 3
    int mudstat;

    validate();

    mudinfo_update_counter++;

    if(BroadcastQueue[mudname]) map_delete(BroadcastQueue, mudname);

    if(single && sizeof(single) && !undefinedp(connected_muds[single])){
        audience = ({ single });
    }
    else audience = keys(connected_muds);


    //trr("audience: "+identify(audience),"white");
    //trr("mud: "+mudname);

    if(mudinfo[mudname]){
        if(undefinedp(connected_muds[mudname]) && mudinfo[mudname]){
            //trr("disconnect time for "+mudname+": "+mudinfo[mudname]["disconnect_time"]);
            if(!mudinfo[mudname]["disconnect_time"] && mudinfo[mudname]["connect_time"]) mudstat = -1;
            else mudstat = 0;
        }
        else {
            //trr("evidently i think we are connected.");
            mudstat = -1;
        }
        if(!mapp(mudinfo[mudname])){
            //trr("WTF. NOT A MAPPING: "+get_stack(),"red");
            return;
        }
        //trr("mudstat: "+mudstat,"white");
        //trr("mud: "+identify(mudinfo[mudname]),"green");
        //trr("broadcasting mudlist to let them know about "+mudname,"red");
        packet1 = ({ "mudlist", 5, router_name, 0, 0, 0, mudinfo_update_counter,([
                    mudname:({
                        mudstat,
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
        packet3 = ({ "mudlist", 5, router_name, 0, 0, 0, mudinfo_update_counter,([
                    mudname:({
                        mudstat,
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
    }
    if(!mudinfo[mudname]){ // deleted mud...
        trr("%^B_RED%^%^BLACK%^No mudinfo for "+mudname);
        // just send a 0 to everyone
        foreach(string targ_mudname in audience){
            write_data(connected_muds[targ_mudname], ({
                        "mudlist", 5, router_name, 0, targ_mudname,
                        0, mudinfo_update_counter, ([mudname:0])
                        }) );
        }
        return;
    }
    if(!remote){
        if(!single){
            //trr("%^RESET%^%^B_MAGENTA%^sending IRN mudstat for "+mudname+": "+identify(mudinfo[mudname]));
            trr("%^RESET%^%^B_MAGENTA%^sending IRN mudstat for "+mudname);
            this_object()->SendList( ([ mudname : mudinfo[mudname] ]) );
        }
    }
    foreach(string targ_mudname in audience){
        int womble;
        if(!sizeof(mudinfo) || !mudinfo[targ_mudname]) continue;
        switch(mudinfo[targ_mudname]["protocol"]){
            case 1:
            case 2:
                packet1[4]=targ_mudname;
                //write_data(connected_muds[targ_mudname], packet1);
                call_out( (: write_data :), 1, connected_muds[targ_mudname],
                        packet1);
                break;
            case 3:
                //trr("I want to write to "+targ_mudname+": "+identify(packet3));
                womble = connected_muds[targ_mudname];
                packet3[4]=targ_mudname;
                //trr("I want to write to "+targ_mudname+": "+identify(packet3));
                //trr("connected_muds["+targ_mudname+"] is: "+womble);
                //trr("this_object()->query_connected_fds()["+womble+"] is: "+
                //this_object()->query_connected_fds()[womble]);

                //write_data(connected_muds[targ_mudname], packet3);
                call_out( (: write_data :), 1, connected_muds[targ_mudname],
                        copy(packet3), womble);
                break;
        }
    }
}

varargs void schedule_broadcast(string mudname, int remote, string single){
    validate();
    if(!BroadcastQueue) BroadcastQueue = ([]);
    if(BroadcastQueue[mudname]) return;
    else { 
        BroadcastQueue[mudname] =([]);
        if(remote) BroadcastQueue[mudname]["remote"] = remote;
        else BroadcastQueue[mudname]["remote"] = 0;
        if(single) BroadcastQueue[mudname]["single"] = single;
        else BroadcastQueue[mudname]["single"] = 0;
        call_out( (: broadcast_mudlist :), 10,
                mudname, BroadcastQueue[mudname]["remote"], 
                BroadcastQueue[mudname]["single"] );
    }
}
