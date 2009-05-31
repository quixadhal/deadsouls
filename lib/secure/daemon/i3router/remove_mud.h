static void validate();
static int *conn_fds = ({});

varargs void remove_mud(mixed mudname, int forced){
    int targetfd;
    validate();
    if(intp(mudname)){
        targetfd = mudname;
        mudname = (this_object()->query_connected_fds()[targetfd] || "");
    }
    else {
        targetfd = connected_muds[mudname];
        if(undefinedp(connected_muds[mudname])) targetfd = -1;
    }
    trr("remove_mud("+identify(mudname)+", "+forced+"), fd: "+targetfd, "red");
    //trr("stack: "+get_stack(),"red");
    if(targetfd > -1 && !forced){
        //trr("Cannot remove mud ["+mudname+"] because it is still online right now.\n");
        return;
    }
    if(targetfd == -1){
        //trr("Warning: cannot disconnect mud ["+mudname+"] because it is not in the connected_muds list.\n");
        server_log("Warning: cannot disconnect mud ["+mudname+"] because it is not in the connected_muds list.");
    }
    else {
        server_log(" Disconnecting mud: "+mudname+" on fd: "+targetfd+"\n");
        //trr(timestamp()+" Disconnecting mud: "+mudname+" on fd: "+targetfd);
    }
    if(!mudinfo[mudname] && forced){         
        //trr("Warning: cannot remove mud ["+mudname+"] because it is not in the mudinfo list.\n");
        server_log(" Warning: cannot remove mud ["+mudname+"] because it is not in the mudinfo list.");
    }
    else{
        server_log("Removing mud: "+mudname+" on fd: "+targetfd+"\n");
        //trr(timestamp()+" Removing mud: "+mudname+" on fd: "+targetfd);
    }

    foreach(mudname in keys(muds_on_this_fd(targetfd))){         
        if(socket_status(targetfd)[1] == "LISTEN") continue;
        //trr(timestamp()+" remove_mud: Removing mud from connected_muds list: "+mudname,"red");
        if(mudinfo[mudname] && !undefinedp(connected_muds[mudname])){ 
            mudinfo[mudname]["disconnect_time"] = time();
            map_delete(connected_muds, mudname);
        }
    }

    conn_fds = keys(this_object()->query_connected_fds());

    if(targetfd != -1){
        mixed sstat = socket_status(targetfd);
        string msg;
        if(sizeof(sstat) < 2 || sstat[0] == -1 || sstat[1] == "CLOSED" ||
                sstat[1] == "LISTEN"){
            return;
        } 
        msg = "About to attempt a disconnect of "+mudname;
        msg += " on "+targetfd+" whose status is "; 
        msg += identify(sstat);
        server_log(msg);
        trr(msg);
        this_object()->close_connection(targetfd);
        server_log("Removing mud: "+mudname+" on fd: "+targetfd);
        if(sizeof(mudname)){
            SendList( ([ mudname : 0 ]), 0, "mudlist" );
            schedule_broadcast(mudname);
        }
    }
    if(forced){
        server_log("Deleting "+mudname+" from mud list!");
        map_delete(mudinfo, mudname);
    }
    //server_log("Removing mud: "+mudname+" on fd: "+targetfd);
    //SendList( ([ mudname : 0 ]), 0, "mudlist" );
    //schedule_broadcast(mudname);
}

varargs void disconnect_mud(string mudname, int remote){
    validate();
    if(mudinfo[mudname]){
        mudinfo[mudname]["disconnect_time"] = time();
        mudinfo[mudname]["connect_time"] = 0;
        if(!undefinedp(connected_muds[mudname])){
            close_connection(connected_muds[mudname]);
            map_delete(connected_muds, mudname);
        }
        schedule_broadcast(mudname, remote);
    }
}
