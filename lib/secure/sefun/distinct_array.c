//    /adm/simul_efun/distinct_array.c
//    from Dead Souls
//    a faster, better namedfunction based on Huthar's uniq_array()
//    by Descartes of Borg 940117

mixed *distinct_array(mixed *arr) {
    mapping tmp;
    int i, maxi;

    for(i = 0, tmp = allocate_mapping(maxi = sizeof(arr)); i<maxi; i++)
      tmp[arr[i]] = 1;
    return keys(tmp);
}
