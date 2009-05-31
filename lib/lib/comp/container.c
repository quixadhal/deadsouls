/*    /lib/comp/container.c
 *    From the Dead Souls LPC Library
 *    Object inherited by all objects with inventories
 *    Created by Descartes of Borg 940212
 *    Version: @(#) container.c 1.3@(#)
 *    Last modified: 96/12/31
 */

#include <lib.h>

inherit LIB_LOOK_IN;
inherit LIB_ADDSTUFF;

int SetOpacity(int x){
    return look_in::SetOpacity(x);
}

int GetOpacity(){
    return look_in::GetOpacity();
}

int GetRadiantLight(int ambient){
    int r = this_object()->GetBaseRadiance(ambient);
    int o = GetOpacity();
    if( o > 99 ){
        if( r < 1 ){
            return 0;
        }
        else {
            int y = r / (ambient || 1);

            if( y > r ){
                return r;
            }
            else {
                return y;
            }
        }
    }
    foreach(object ob in all_inventory()){
        r += ob->GetRadiantLight(ambient);
    }
    if( ambient > 0 ){
        int y;

        y = (r*10)/ambient;
        if( y > r ){
            y = r;
        }
        else {
            r = y;
        }
    }
    return ( (r*(100-o))/100 );
}

int CanReceive(object ob){
    return 1;
}

int CanRelease(object ob){
    return 1;
}

int eventReceiveObject(object ob){
    return !(!previous_object());
}

int eventReleaseObject(object ob){
    return !(!previous_object());
}

mixed eventPostRelease(object ob){
    return 1;
}
