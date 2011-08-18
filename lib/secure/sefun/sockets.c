string dump_socket_status() {
    string ret;
    string *finalsocks, *sock_array = ({});
    int i, quant = sizeof(socket_status());
    for(i = 0; i < quant; i++){
        sock_array += ({ socket_status(i) });
    }
    finalsocks = sock_array;

    ret = "";
    ret += "        Fd    State      Mode       Local Address          Remote Address";
    ret += "        --  ---------  --------  ---------------------  ---------------------";

    foreach (array item in finalsocks) {
        int memb = member_array(item, finalsocks);
        ret += sprintf("%2d  %|9s  %|8s  %-21s  %-21s\n", memb, item[1], item[2], item[3], item[4]);
    }

    return ret;
}

mixed *socket_names(){
    string *finalsocks, *sock_array = ({});
    int i, quant;
    i = catch( quant = sizeof(socket_status()) );
    if(i){
        return ({});
    }
    for(i = 0; i < quant; i++){
        mixed *tmp = socket_status(i);
        tmp[0] = i;
        sock_array += ({ tmp });
    }
    finalsocks = sock_array;

    return finalsocks;
}

string socket_ip(int fd){
    string ret = "0";
    mixed sock_stat = socket_status(fd);
    if(sock_stat) ret = implode(explode(sock_stat[4],".")[0..3], ".");
    return ret;
}
