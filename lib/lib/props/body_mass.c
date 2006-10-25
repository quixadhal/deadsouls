/*    /lib/props/mass.c
 *    From the Dead Souls Object Library
 *    Handles object massiveness and weight
 *    Created by Descartes of Borg 970101
 *    Version: @(#) mass.c 1.1@(#)
 *    Last modified: 97/01/01
 */

#include <config.h>

int BodyMass = 0;

int AddBodyMass(int x) {
    if( BodyMass + x > 0 ) {
	x = -BodyMass;
    }
    if( environment() ) {
	environment()->AddCarriedBodyMass(x);
    }
    return (BodyMass -= x);
}

int GetBodyMass() {
    int load = this_object()->GetCarriedBodyMass();
    return (BodyMass + load);
}

int SetBodyMass(int x) {
    if( x < 0 ) {
	x = 0;
    }
    return (BodyMass = x);
}

int GetWeight() {
    float h;

    if( environment() ) {
	h = environment()->GetGravity();
    }
    else {
	h = DEFAULT_GRAVITY;
    }
    return to_int(GetBodyMass() * h);
}

int GetMass(){
    return GetBodyMass();
}

