/*
 * Brodbane@Eve 03/03/06
 * Adapted from original by Caliel@BloodRose
*/
varargs string *wild_card(string str) {
    mixed pf; string *tmp, cwd;
    if((cwd = absolute_path("cwd",str)) == "/") return ({ "/" });
    if((string)(pf= path_file(cwd)) == "/") pf[0] = "";
    if(!(tmp = get_dir(cwd))) tmp = ({});
    tmp -= ({ ".", ".." });
    for(int i=0;i<sizeof(tmp);i++) {
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


