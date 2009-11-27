string *bitses = ({});

int bool_reverse(mixed arg){
    if(!arg || (intp(arg) && arg < 1)) return 1;
    else return 0;
}

int atoi(string str){
    int integer;
    if(str) sscanf(str, "%d", integer);
    return integer;
}

string itoa(mixed i){
    return ""+i;
}

string comma(mixed x){
    string ret = "", pre = "", post = "";
    int count = 0, i;
    if(floatp(x)) sscanf(x+"","%s.%s",pre,post);
    else if(intp(x)) pre = itoa(x);
    else return 0;
    i = sizeof(pre);
    while(i){
        count++;
        ret = pre[<count..<count] + ret;
        if(!(count % 3) && (i = sizeof(pre[<count+1..<count+1]))){
            ret = "," + ret;
        }
    }
    if(!sizeof(ret)) ret = "0";
    if(sizeof(post)){
        ret += "." + post[0..1];
    }
    return ret;
}

int bitshiftedp(int x){
    int i;
    if(!sizeof(bitses)){ ;
        for(i=0;i<32;i++){
            bitses += ({ ( 1 << i ) });
        }
    }
    if(member_array(x,bitses) != -1) return 1;
    else return 0;
}

int *roll(string alea){
    int i,*ret,quot,quod;
    if( !sscanf(alea,"d%d",quod )){
        if(sscanf(alea,"%dd%d",quot,quod) != 2){
            return 0;
        }
    }
    else quot = 1;

    ret = allocate(quot+1);
    for(i=0;i<quot;i++){
        ret[i+1] = random(quod)+1;
        ret[0] += ret[i+1];
    }

    return ret;
}

