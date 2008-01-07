mixed *scramble_array(mixed *arr){
    mixed *ret = copy(arr);
    ret = sort_array(ret, (:random(3)-1:) );
    return ret;
}

mixed *distinct_array(mixed *arr) {
    mapping tmp;
    int i, maxi;

    for(i = 0, tmp = allocate_mapping(maxi = sizeof(arr)); i<maxi; i++)
        tmp[arr[i]] = 1;
    return keys(tmp);
}

mixed *singular_array(mixed *arr) {
    return distinct_array(arr);
}

