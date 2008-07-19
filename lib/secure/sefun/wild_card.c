string *wild_card(string str) {
    mixed pf;
    string *tmp, cwd;
    int i, maxi;
    if((cwd = absolute_path(this_player()->query_cwd(), str)) == "/") 
        return ({ "/" });
    if(cwd[sizeof(cwd) - 1] == '/') cwd = cwd[0..sizeof(cwd) - 2];
    if((string)(pf = path_file(cwd)) == "/") pf[0] = "";
    if(!(tmp = get_dir(cwd))) tmp = ({});
    tmp -= ({ "." });
    tmp -= ({ ".." });
    if(!str || str[0] != '.')
        tmp = filter(tmp, "remove_dots", this_object());
    for(i=0, maxi = sizeof(tmp); i < maxi; i++) {
        if(file_size(sprintf("%s/%s", pf[0], pf[1])) == -2) tmp[i] = pf[0];
        else tmp[i] = sprintf("%s/%s", pf[0], tmp[i]);
        if(strlen(tmp[i]) > 1 && tmp[i][0..1] == "//")
            tmp[i] = tmp[i][1..strlen(tmp[i])-1];
    }
    return tmp;
}

nomask int remove_dots(string str){ 
    return !(str[0] == '.'); 
}


