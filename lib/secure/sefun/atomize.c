string *atomize_string(string str){
    int i;
    string *ret = ({});
    for(i=sizeof(str);i > 0;i--){
        ret += ({ str[0..i] });
    }
    return ret;
}

string *atomize_array(string *temp_arr){
    string *ret = ({});
    foreach(string element in temp_arr){
        ret += atomize_string(element);
    }
    ret += temp_arr;
    return distinct_array(ret);
}
