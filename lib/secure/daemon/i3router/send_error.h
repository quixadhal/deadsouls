// This file written mostly by Tim Johnson (Tim@TimMUD)

static void send_error(string mud, string user, string errcode, string errmsg, mixed *info){
    if(undefinedp(connected_muds[mud])){
        //trr("Can't send error to "+mud+" because they're not connected.");
        return;
    }
    server_log("Sending error to "+mud+": "+errmsg);
    write_data(connected_muds[mud],({
                "error",
                5,
                router_name,
                0,
                mud, // mud name
                user, // user name
                errcode,
                errmsg,
                info
                }));
    return;
}
