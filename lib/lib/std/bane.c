string array bane;

string array GetBane(){
    if(bane) return bane;
}

string array QueryBane(){
    if(bane) return bane;
}

int SetBane(array arr){
    if(arr) bane = arr;
    return 1;
}
