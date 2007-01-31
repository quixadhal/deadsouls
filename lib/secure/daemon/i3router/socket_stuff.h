// This file written in great part by Tim Johnson (Tim@TimMUD)

static void close_callback(int fd){
    string mudname;
    trr("close_callback: fd="+fd+"\n");
    if(socket_status(fd)[1] == "LISTEN") return;
    foreach(mudname in keys(muds_on_this_fd(fd))){
	trr(timestamp()+" close_callback: Removing mud from connected_muds list: "+mudname,"red");
	if(mudinfo[mudname]) mudinfo[mudname]["disconnect_time"] = time();
	map_delete(connected_muds, mudname);
	broadcast_mudlist(mudname);
    }
    close_connection(fd);
}

static void listen_callback(int fd){
    int fdstat;
    trr("listen_callback: socket_status("+fd+"): "+identify(socket_status(fd)));
    if ((fdstat = socket_accept(fd, "read_callback", "write_callback")) < 0) {
	trr("listen_callback couldn't accept socket "+fd+", errorcode "+fdstat);
	return;
    }
    else trr("listen_callback: fdstat: "+fdstat);
}

void write_callback(int fd){
    if (sockets[fd]["write_status"] == EEALREADY) {
	write_data(fd, sockets[fd]["pending"]);
	map_delete(sockets[fd], "pending");
    } 
    else {
	sockets[fd]["write_status"] = EESUCCESS;
    }
}

void write_data_retry(int fd, mixed data, int counter){
    int rc;
    //trr("write_data_retry, fd("+fd+"), "+this_object()->query_connected_fds()[fd]+
    //", counter: "+counter);

    if (counter == MAXIMUM_RETRIES) {
	//close_connection(fd);
	trr("Could not write data to "+this_object()->query_connected_fds()[fd]+
	  " "+identify(data));
	return;
    }
    rc = socket_write(fd, data);
    if(!sockets[fd]) sockets[fd]=([]); // Tim
    sockets[fd]["write_status"] = rc;
    switch (rc) {
    case EESUCCESS:
	break;
    case EEALREADY:
	sockets[fd]["pending"] = data;
	break;
    case EECALLBACK:
	break;
    default:
	if (counter < MAXIMUM_RETRIES) {
	    trr("write_data_retry: " + socket_error(rc));
	    //call_out("retry_write", 2, ({fd, data, counter + 1}));
	    //call_out( (: write_data_retry :), 2 , $(fd), $(data), $(counter) + 1 ); 
	    call_out( (: write_data_retry :), 2 , fd, data, counter + 1 ); 
	    return;
	}
    }
}

static void close_connection(int fd){
    int sockerr;
    if(socket_status(fd)[1] == "LISTEN") return;
    map_delete(sockets, fd);
    trr("About to try closing socket: "+fd,"yellow");
    trr("Pre-closing state: "+socket_status(fd)[1],"yellow");
    sockerr = socket_close(fd);
    trr("closing socket:"+fd,"white");
    trr("closing sockerr:"+sockerr,"white");
    trr("Post-closing state: "+socket_status(fd)[1],"yellow");
}

static void write_data(int fd, mixed data){
    write_data_retry(fd, data, 0);
}

static void broadcast_data(mapping targets, mixed data){
    foreach(int *arr in unique_array(values(targets), (: $1 :))){
	write_data(arr[0], data);
    }
}
