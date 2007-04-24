#include <virtual.h>

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

