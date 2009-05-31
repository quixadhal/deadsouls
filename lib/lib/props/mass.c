/*    /lib/props/mass.c
 *    From the Dead Souls Mud Library
 *    Handles object massiveness and weight
 *    Created by Descartes of Borg 970101
 *    Version: @(#) mass.c 1.1@(#)
 *    Last modified: 97/01/01
 */

int Mass = 0;

int AddMass(int x){
    if( Mass + x > 0 ){
        x = -Mass;
    }
    if( environment() ){
        environment()->AddCarriedMass(x);
    }
    return (Mass -= x);
}

int GetMass(){
    int load = this_object()->GetCarriedMass();
    return (Mass + load);
}

int SetMass(int x){
    if( x < 0 ){
        x = 0;
    }
    return (Mass = x);
}

string array GetSave(){
    return ({ "Mass" });
}

int GetWeight(){
    float h;

    if( environment() ){
        h = environment()->GetGravity();
    }
    else {
        h = DEFAULT_GRAVITY;
    }
    return to_int(GetMass() * h);
}

