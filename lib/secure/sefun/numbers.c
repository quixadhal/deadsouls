
int bool_reverse(mixed arg){
    if(!arg || (intp(arg) && arg < 1)) return 1;
    else return 0;
}

int atoi(string str){
    int integer;
    if(!str) return 0;
    if(!sscanf(str, "%d", integer)) return 0;
    return integer;
}
