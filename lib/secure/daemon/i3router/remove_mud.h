// This file written completely by Tim Johnson (Tim@TimMUD)

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
    if(!mudinfo[mudname]){         
        trr("Warning: cannot remove mud ["+mudname+"] because it is not in the mudinfo list.\n");
log_file("router/server_log",timestamp()+" Warning: cannot remove mud ["+mudname+"] because it is not in the mudinfo list.\n");
    }
    else{
    log_file("router/server_log",timestamp()+" Removing mud: "+mudname+" on fd:
"+targetfd+"\n");
    trr(timestamp()+" Removing mud: "+mudname+" on fd: "+targetfd);
    }
    map_delete(mudinfo, mudname);
    close_callback(targetfd);
    log_file("router/server_log",timestamp()+" Removing mud: "+mudname+" on fd: "+targetfd+"\n");
    //if(connected_muds[mudname]) map_delete(connected_muds, mudname);
}
