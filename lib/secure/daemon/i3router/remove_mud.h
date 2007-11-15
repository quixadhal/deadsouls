static void validate();
static int *conn_fds = ({});

varargs void remove_mud(string mudname, int forced){
    int targetfd = connected_muds[mudname];
    validate();
    //trr("stack: "+get_stack(),"red");
    if(connected_muds[mudname] && !forced){
        //trr("Cannot remove mud ["+mudname+"] because it is still online right now.\n");
        return;
    }
    if(!connected_muds[mudname]){
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
        if(mudinfo[mudname] && connected_muds[mudname]){ 
            mudinfo[mudname]["disconnect_time"] = time();
            map_delete(connected_muds, mudname);
        }
    }

    conn_fds = keys(this_object()->query_connected_fds());

    if(member_array(targetfd, conn_fds) != -1){
        string msg = "About to attempt a disconnect of "+mudname;
        msg += " on "+targetfd+" whose status is "; 
        msg += identify(socket_status(targetfd));
        server_log(msg);
        trr(msg);
        this_object()->close_connection(targetfd);
    }
    if(forced){
        server_log("Deleting "+mudname+" from mud list!");
        map_delete(mudinfo, mudname);
    }
    server_log("Removing mud: "+mudname+" on fd: "+targetfd);
    SendList( ([ mudname : 0 ]), 0, "mudlist" );
    schedule_broadcast(mudname);
}

varargs void disconnect_mud(string mudname, int remote){
    validate();
    if(mudinfo[mudname] && connected_muds[mudname]){
        mudinfo[mudname]["disconnect_time"] = time();
        mudinfo[mudname]["connect_time"] = 0;
        close_connection(connected_muds[mudname]);
        map_delete(connected_muds, mudname);
        schedule_broadcast(mudname, remote);
    }
}
