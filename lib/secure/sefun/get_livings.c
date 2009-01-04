/*    /secure/sefun/get_livings.c
 *    From the Frontiers LPC Library
 *	an ugly hack to return an array of living things. 
 *    Give it an integer argument to return interactives only
 *    Created by Cratylus 23JAN2005
 *    Version: @(#) getlivings.c 1.1@(#)
 */

varargs object array get_livings(object ob,int foo){
    object *stuff,*lstuff,*istuff;
    int i;
    if(!ob) return ({});
    stuff=all_inventory(ob);
    lstuff = ({});
    for(i=0;i<sizeof(stuff);i++){
        if(living(stuff[i]) && !sizeof(lstuff)) lstuff = ({stuff[i]});
        if(living(stuff[i]) && sizeof(lstuff) > 0 &&
                member_array(stuff[i],lstuff) == -1) lstuff += ({stuff[i]});
    }

    if(foo == 1){
        istuff=({});
        for(i=0;i<sizeof(lstuff);i++){
            if( interactive(lstuff[i]) && !sizeof(istuff) ) istuff = ({lstuff[i]});
            if( interactive(lstuff[i]) && sizeof(istuff)> 0 &&
                    member_array(lstuff[i],istuff) == -1) istuff+= ({lstuff[i]});
        }
        if(sizeof(istuff) > 0) return istuff;
        if(!sizeof(istuff)) return 0;
    }

    if(foo == 2){
        istuff=({});
        for(i=0;i<sizeof(lstuff);i++){
            if( !interactive(lstuff[i]) && !sizeof(istuff) ) istuff = ({lstuff[i]});
            if( !interactive(lstuff[i]) && sizeof(istuff)> 0 &&
                    member_array(lstuff[i],istuff) == -1) istuff+= ({lstuff[i]});
        }
        if(sizeof(istuff) > 0) return istuff;
        if(!sizeof(istuff)) return 0;
    }

    if(sizeof(lstuff) > 0)      return lstuff;
    if(!sizeof(lstuff)) return 0;
}

varargs object get_random_living(object room, int foo){
    object *livings;

    if(!foo) foo = 0;

    livings = get_livings(room, foo);
    foo = random(sizeof(livings));

    return livings[foo];
}





