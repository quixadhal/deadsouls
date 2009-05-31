string convert_ascii(int i){
    return sprintf("%c",i);
}

mixed convert_into_ascii(string str){
    int i;
    string *tmparr = ({});
    int *intarr = ({});
    if(!str) return 0;
    tmparr = explode(str,"");
    foreach(string element in tmparr){
        for(i=0;i<256;i++){
            if(sprintf("%c",i) == element) intarr += ({ i });
        }
    }
    return intarr;
}
