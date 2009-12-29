int levenshtein_distance(string a, string b) {
    int alen = sizeof(a);
    int blen = sizeof(b);
    mixed foo, baz = alen+1;
    mixed array dist = ({});
#ifdef __FLUFFOS__
    while(baz){
        dist += ({ allocate(blen +1) });
        baz--;
    } 
    foreach(foo in dist){
        foo[0] = baz;
        baz++;
    }
    baz = 1;
    while(baz < (blen + 1)){
        dist[0][baz] = baz;
        baz++;
    }
    for(int i = 0; i < alen; i++){
        for(int j = 0; j < blen; j++){
            dist[i + 1][j + 1] = min( ({ dist[i][j + 1] + 1, dist[i + 1][j] + 1, dist[i][j] + (a[i] != b[j]) }) );
        }
    }
    return dist[alen][blen];
#else
    return 1;
#endif
}

int damerau_levenshtein_distance(string a, string b) {
    int alen = sizeof(a);
    int blen = sizeof(b);
    mixed foo, baz = alen+1;
    mixed array dist = ({});
#ifdef __FLUFFOS__
    while(baz){
        dist += ({ allocate(blen +1) });
        baz--;
    }
    foreach(foo in dist){
        foo[0] = baz;
        baz++;
    }
    baz = 1;
    while(baz < (blen + 1)){
        dist[0][baz] = baz;
        baz++;
    }
    for(int i = 0; i < alen; i++)
        for(int j = 0; j < blen; j++)
            if(i && j && a[i] == b[j - 1] && a[i - 1] == b[j])
                dist[i + 1][j + 1] = min( ({ dist[i][j + 1] + 1, dist[i + 1][j] + 1, dist[i][j] + (a[i] != b[j]), dist[i - 1][j - 1] + (a[i] != b[j]) }) );
            else
                dist[i + 1][j + 1] = min( ({ dist[i][j + 1] + 1, dist[i + 1][j] + 1, dist[i][j] + (a[i] != b[j]) }) );
    return dist[alen][blen];
#else
    return 1;
#endif
}
