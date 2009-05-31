// This file written mostly by Tim Johnson (Tim@TimMUD)

static void send_chanlist_reply(string mudname, int old_chanid){
    string channame; // channel name
    //	string *output; // array of which channel names to tell them about
    mapping out=([ ]); // mapping to send
    //int i; // counter
    //trr("send_chanlist_reply, mudname="+mudname+", old_chanid="+old_chanid);
    foreach(channame in keys(channel_updates)){
        //#ifndef SEND_WHOLE_CHANLIST
        //	if(channel_updates[channame]>old_chanid){ // needs updating
        //#endif
        if(!channels[channame]){ // add to output;
            out[channame] = 0;
        }
        else{
            out[channame] = ({ channels[channame][1],
                    channels[channame][0] }); // host, type
        }
        //#ifndef SEND_WHOLE_CHANLIST
        //	}
        //#endif
    }
    //trr("Chanlist: "+identify(out),"blue");
    write_data(connected_muds[mudname],({
                "chanlist-reply",
                5,
                router_name,
                0,
                mudname,
                0,
                channel_update_counter,
                out,
                }));
    mudinfo[mudname]["old_chanlist_id"]=channel_update_counter;
}
