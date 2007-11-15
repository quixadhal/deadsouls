//	/adm/simul_efun/parse_objects.c
//	from the Dead Souls mudlib
//	searches the inventory of an object for a numbered object
//	created by Descartes of Borg 25 october 1992

object parse_objects(object where, string str) {
    int i, skip, which;
    string what;
    object *inv;

    if(!str) return 0;
    if(sscanf(str, "%s %d", what, which) != 2) return 0;
    inv = all_inventory(where);
    for(i=0, skip = 0; i<sizeof(inv); i++) {
        if(inv[i]->id(what)) skip ++;
        if(skip == which) return inv[i];
    }
    return 0;
}
