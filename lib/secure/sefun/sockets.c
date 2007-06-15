string dump_socket_status() {
    string ret;
    string *finalsocks, *sock_array = ({});
    int i, quant = sizeof(socket_status());
    for(i = 0; i < quant; i++){
	sock_array += ({ socket_status(i) });
    }
    finalsocks = sock_array;

    ret = @END
Fd    State      Mode       Local Address          Remote Address
--  ---------  --------  ---------------------  ---------------------
END;

    foreach (array item in finalsocks) {
	int memb = member_array(item, finalsocks);
	ret += sprintf("%2d  %|9s  %|8s  %-21s  %-21s\n", memb, item[1], item[2], item[3], item[4]);
    }

    return ret;
}

mixed *socket_names(){
    string *finalsocks, *sock_array = ({});
    int i, quant;
    //tc("About to try a socket_status().","yellow");
    i = catch( quant = sizeof(socket_status()) );
    //tc("I survived. i is: "+i,"green");
    for(i = 0; i < quant; i++){
	mixed *tmp = socket_status(i);
        //tc("Made it into the for. i is: "+i,"white");
	tmp[0] = i;
	sock_array += ({ tmp });
    }
    finalsocks = sock_array;

    return finalsocks;
}
