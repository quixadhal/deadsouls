// This file written in great part by Tim Johnson (Tim@TimMUD)

static void close_callback(int fd){
    string mudname;
    tn("close_callback: fd="+fd+"\n");
    foreach(mudname in keys(muds_on_this_fd(fd))){
	tn("Removing mud from connected_muds list: "+mudname);
	mudinfo[mudname]["disconnect_time"] = time();
	map_delete(connected_muds, mudname);
	broadcast_mudlist(mudname);
    }
    close_connection(fd);
}

static void listen_callback(int fd){
    int fdstat;
    if ((fdstat = socket_accept(fd, "read_callback", "write_callback")) < 0) {
	return;
    }
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

static void write_data_retry(int fd, mixed data, int counter){
    int rc;
    if (counter == MAXIMUM_RETRIES) {
	close_connection(fd);
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
    case EESEND:
	if (counter < MAXIMUM_RETRIES) {
	    tn("retry hit","cyan");
	    call_out("retry_write", 2, ({fd, data, counter + 1}));
	    return;
	}
    default:
	tn("write_data_retry: " + socket_error(rc) + "yellow");
	close_connection(fd);
	break;
    }
}

static void close_connection(int fd){
    int sockerr;
    map_delete(sockets, fd);
    sockerr = socket_close(fd);
    tn("closing sockerr:"+sockerr,"green");
}

static void write_data(int fd, mixed data){
    write_data_retry(fd, data, 0);
}

static void broadcast_data(mapping targets, mixed data){
    foreach(int *arr in unique_array(values(targets), (: $1 :))){
	write_data(arr[0], data);
    }
}
