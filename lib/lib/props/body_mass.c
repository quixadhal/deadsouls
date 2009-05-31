#include <lib.h>
#include <daemons.h>

string GetRace();

int BodyMass = 0;

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
    int base_mass = RACES_D->GetRaceMass(GetRace());
    int load = this_object()->GetCarriedMass();
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

    if( environment() ){
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
