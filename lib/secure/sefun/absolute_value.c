//      /adm/simul_efun/absolute_value.c
//      from the Dead Souls mudlib
//      an absolute value simul_efun
//      created by Descartes 28 april 1993

int absolute_value(int x) {
    return ( (x>-1) ? x : -x);
}

int abs(int x) {
    return absolute_value(x);
}
