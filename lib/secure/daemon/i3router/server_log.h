varargs void server_log(string str, string file){
    trr(str+"%^RESET%^");
    str = strip_colours(str);
    if(!file){
        return log_file("router/server_log", timestamp()+" "+str+"\n");
    }
    return log_file("router/"+file, timestamp()+" "+str+"\n");
}
