void send_full_mudlist(string mud){
    int interval = 0;
    mixed *package = ({});
    mixed *names = ({});
    if(undefinedp(connected_muds[mud])) return;
    //trr("trying to send full mudlist to "+mud);
    foreach(string mudname in keys(mudinfo)){
        int mudstat;
        if(undefinedp(connected_muds[mudname]) && mudinfo[mudname]){
            if(!mudinfo[mudname]["disconnect_time"]) mudstat = -1;
            else mudstat = 0;
        }
        else {
            mudstat = -1;
        }

        package += ({ ({ "mudlist", 5, router_name, 0, 0, 0, mudinfo_update_counter,([
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
                        ]) }) });
    }
    while(sizeof(package)){
        //trr("package: "+sizeof(package),"white");
        interval++;
        foreach(mixed element in package[0..9]){
            string name = identify(keys(element[7]));
            names += ({ name });
            //trr("element: "+identify(element));
            call_out( (: write_data :), interval, connected_muds[mud], element);
            //trr("sending info on "+name+" to "+mud+", interval: "+interval,"cyan");
        }
        package -= package[0..9];
    }
    //trr("number of muds: "+sizeof(mudinfo),"red");
    //trr("number of muds sent: "+sizeof(names),"red");
    //trr("muds: "+identify(names),"red");
}
