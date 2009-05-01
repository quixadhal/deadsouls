// This file written mostly by Tim Johnson (Tim@TimMUD)

// broadcast the chanlist-reply when a channel just now got changed...
varargs void broadcast_chanlist(string channame, string thismud){
    string name; // channel name in first loop, then mud name in later loop
    string *mudses = keys(connected_muds);
    mapping out=([]); // only used if whole chanlist is sent
    if(thismud && thismud != "") mudses = ({ thismud });
#ifdef SEND_WHOLE_CHANLIST
    //trr("broadcasting chanlist to let them know about "+channame);
    foreach(name in keys(channels)){
        out[name]=({ channels[name][1], channels[name][0] });
    }
#else
    if(!channels[channame])
        out = ([ channame:0 ]);
    else
        out = ([ channame:({ channels[channame][1], channels[channame][0] }) ]);
#endif
    foreach(name in mudses){
        trr("inside loop with "+name);
        if(member_array("channel", keys(mudinfo[name]["services"]))!=-1){
            // only tell muds that have the "channel" service...
            write_data(connected_muds[name], ({
                        "chanlist-reply",5,router_name,0,name,0,channel_update_counter,out
                        }));
        }
        //trr("done with: "+name);
    }
}
