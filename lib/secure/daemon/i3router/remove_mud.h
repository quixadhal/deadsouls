static void validate();

varargs void remove_mud(string mudname, int forced){
    int targetfd = connected_muds[mudname];
    validate();
    trr("stack: "+get_stack(),"red");
    if(connected_muds[mudname] && !forced){
	trr("Cannot remove mud ["+mudname+"] because it is still online right now.\n");
	return;
    }
    if(!connected_muds[mudname]){
	trr("Warning: cannot disconnect mud ["+mudname+"] because it is not in the connected_muds list.\n");
	log_file("router/server_log",timestamp()+" Warning: cannot disconnect mud ["+mudname+"] because it is not in the connected_muds list.\n");
    }
    else {
	log_file("router/server_log",timestamp()+" Disconnecting mud: "+mudname+" on fd:
"+targetfd+"\n");
    trr(timestamp()+" Disconnecting mud: "+mudname+" on fd: "+targetfd);
    }
    if(!mudinfo[mudname] && forced){         
        trr("Warning: cannot remove mud ["+mudname+"] because it is not in the mudinfo list.\n");
log_file("router/server_log",timestamp()+" Warning: cannot remove mud ["+mudname+"] because it is not in the mudinfo list.\n");
    }
    else{
    log_file("router/server_log",timestamp()+" Removing mud: "+mudname+" on fd:
"+targetfd+"\n");
    trr(timestamp()+" Removing mud: "+mudname+" on fd: "+targetfd);
    }
    foreach(mudname in keys(muds_on_this_fd(targetfd))){         
if(socket_status(targetfd)[1] == "LISTEN") continue;
trr(timestamp()+" remove_mud: Removing mud from connected_muds list: "+mudname,"red");
        if(mudinfo[mudname]) mudinfo[mudname]["disconnect_time"] = time();
        map_delete(connected_muds, mudname);
        broadcast_mudlist(mudname);
    }
    close_connection(targetfd);
    if(forced) map_delete(mudinfo, mudname);
    log_file("router/server_log",timestamp()+" Removing mud: "+mudname+" on fd: "+targetfd+"\n");
}
