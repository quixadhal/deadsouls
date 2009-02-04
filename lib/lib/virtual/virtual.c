#include <virtual.h>
int exempt_virtual;

int GetVirtual(){
    return 1;
}

int GetVirtualSky(){
    if(inherits(LIB_VIRT_SKY, this_object())) return 1;
    return 0;
}

int GetVirtualSea(){
    if(inherits(LIB_VIRT_SURFACE, this_object())) return 1;
    if(inherits(LIB_VIRT_SUBSURFACE, this_object())) return 1;
    return 0;
}

/* This is to exempt the room from the normal rules
 * of virtuals, including automatic destruction when
 * the last item in its inventory leaves.
 */
int SetExemptVirtual(int i){
    if(i) exempt_virtual = 1;
    else exempt_virtual = 0;
    return exempt_virtual;
}

int GetExemptVirtual(){
    return exempt_virtual;
}
