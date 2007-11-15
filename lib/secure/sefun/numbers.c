string *bitses = ({});

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

string itoa(int i){
    return ""+i;
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

