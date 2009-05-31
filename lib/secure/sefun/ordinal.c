//      /adm/simul_efun/ordinal.c
//      from the Dead Souls Mudlib
//      returns a string with the ordinal suffix for an int 
//      created by Descartes of Borg 03 july 1993

string ordinal(int x) {
    if( x<14 && x>10 ) x = 4;
    else x = x%10;
    switch(x) {
        case 1: return "st"; break;
        case 2: return "nd"; break;
        case 3: return "rd"; break;
        default: return "th"; break;
    }
}
