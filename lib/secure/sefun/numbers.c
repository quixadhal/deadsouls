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
    //tc("bitses is "+sizeof(bitses)+" and is "+identify(bitses));
    if(member_array(x,bitses) != -1) return 1;
    else return 0;
}
