#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mapping Momentum = ([]);

void create(){
    if(!Momentum) Momentum = ([]);
}

#if 0
varargs int AddMomentum(object what, object from, object to, int speed){
    int x1, y1, z1, x2, y2, z2;
    int a, b;
    if(previous_object() != what) return 0;
    if(from == to) return 0;
    if(!from || !to || !speed) return 0;
    a = sscanf(ROOMS_D->GetCoordinates(from,"%d,%d,%d",x1,y1,z1);    
            b = sscanf(ROOMS_D->GetCoordinates(to,"%d,%d,%d",x2,y2,z2);    
                if(a < 3 || b < 3) return 0;
                if(x1 == x2 && y1 == y2 && z1 == z2) return 0;
                if(!Momentum[what]) Momentum[what] = ([ "dir" : 0, "speed" : 0 ]); 
                newdir = bearing(x1, y1, x2, y2);
                olddir = Momentum[what]["dir"];
                oldspeed = Momentum[what]["speed"];
#endif



