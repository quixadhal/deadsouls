/*    /adm/simul_efun/copy.c
 *    from Dead Souls
 *    returns a copy of an array or mapping rather than that array or mapping
 *    created by Descartes of Borg 940204
 *    based upon an older simul_efun of the same name by Buddha@TMI-2
 */

#ifndef __FLUFFOS__
mixed copy(mixed val) {
    mixed *cles;
    mixed ret;
    int i, maxi;

    if(mapp(val)) {
        ret = allocate_mapping(maxi = sizeof(cles = keys(val)));
        for(i= 0; i< maxi; i++) ret[cles[i]] = copy(val[cles[i]]);
        return ret;
    }
    else if(pointerp(val)) {
        ret = allocate(maxi = sizeof(val));
        for(i=0; i<maxi; i++) ret[i] = copy(val[i]);
        return ret;
    }
    return val;
}
#endif
