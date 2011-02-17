#include <lib.h>
#include <daemons.h>

string GetRace();

int BodyMass = 0;
static int zpg = 0;

int SetZPG(int x){
    if(x > 0) zpg = 1;
    else zpg = 0;
}

int GetZPG(){
    return zpg;
}

int AddBodyMass(int x){
    if( BodyMass + x > 0 ){
        x = -BodyMass;
    }
    if( environment() ){
        environment()->AddCarriedBodyMass(x);
    }
    return (BodyMass -= x);
}

int GetBodyMass(){
    int base_mass, load;
    if(GetZPG()) return 0;
    base_mass = RACES_D->GetRaceMass(GetRace());
    load = this_object()->GetCarriedMass();
    return (base_mass + load);
}

int SetBodyMass(int x){
    if( x < 0 ){
        x = 0;
    }
    return (BodyMass = x);
}

int GetWeight(){
    float h;
    object env = room_environment();

    if(env){
        h = env->GetGravity();
    }
    else {
        h = DEFAULT_GRAVITY;
    }
    return to_int(GetBodyMass() * h);
}

int GetMass(){
    return GetBodyMass();
}
