/*    /lib/comp/container.c
 *    From the Dead Souls LPC Library
 *    Object inherited by all objects with inventories
 *    Created by Descartes of Borg 940212
 *    Version: @(#) container.c 1.3@(#)
 *    Last modified: 96/12/31
 */

#include <lib.h>

inherit LIB_LOOK_IN;
inherit LIB_RADIANCE;

int GetRadiantLight(int ambient) {
    int r = radiance::GetRadiantLight(ambient);
    int o = GetOpacity();
    
    if( o > 99 ) {
	if( r < 1 ) {
	    return 0;
	}
	else {
	    int y = (r*10)/ambient;

	    if( y > r ) {
		return r;
	    }
	    else {
		return y;
	    }
	}
    }
    foreach(object ob in all_inventory()) {
	r += ob->GetRadiantLight(ambient);
    }
    if( ambient > 0 ) {
	int y;
	
	y = (r*10)/ambient;
	if( y > r ) {
	    y = r;
	}
	else {
	    r = y;
	}
    }
    return ( (r*(100-o))/100 );
}

int CanReceive(object ob) {
    return 1;
}

int CanRelease(object ob) {
    return 1;
}

int eventReceiveObject(object ob) {
    return !(!previous_object());
}

int eventReleaseObject(object ob) {
    return !(!previous_object());
}
