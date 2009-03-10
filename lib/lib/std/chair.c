/*    /lib/chair.c
 *    From the Dead Souls Mud Library
 *    A thing you can sit in
 *    Created by Descartes of Borg 961221
 *    Version: @(#) chair.c 1.1@(#)
 *    Last modified: 96/12/21
 */

#include <lib.h>

inherit LIB_SIT;
inherit LIB_SURFACE;

void create(){
    surface::create();
    SetMaxSitters(1);
    SetMaxCarry(100);
    SetMass(500);
}

    int CanGet(object who){
        if(sit::CanGet(who))
            return surface::CanGet(who);
        else return 0;
    }

/* Nothing else needs to happen here */
