mixed *singular_array(mixed *arr) {
    mixed *ret = ({});

    foreach(mixed item in arr){
	if(member_array(item, ret) == -1) ret += ({item});
    }

    return ret;
}
