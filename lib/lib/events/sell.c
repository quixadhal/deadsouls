/*    /lib/events/sell.c
 *    From the Dead Souls Objecdt Library
 *    Handles the buying and selling of items
 *    Created by Descartes of Borg 970101
 *    Version: @(#) sell.c 1.1@(#)
 *    Last modified: 97/01/01
 */

mixed CanSell(object seller){
    return 1;
}

mixed direct_sell_obj_to_liv(){
    if( environment() != this_player() ){
        return "#You don't have that!";
    }
    return CanSell(this_player());
}

mixed indirect_sell_liv_obj(){
    return direct_sell_obj_to_liv();
}
