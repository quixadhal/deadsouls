string clean_fd(string fd){
    if(grepp(fd," ")){
        string *ip_split=explode(fd," ");
        fd = ip_split[0];
    }
    return fd;
}
