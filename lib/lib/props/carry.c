/*    /lib/props/carry.c
 *    From the Dead Souls LPC Library
 *    Object for handling carried goods
 *    Created by Descartes of Borg 950324
 *    Version: @(#) carry.c 1.10@(#)
 *    Last modified: 96/12/22
 */

static int CarriedMass = 0;
int MaxCarry    = 0;

int CanCarry(int amount);

int AddCarriedMass(int x){
    if( !CanCarry(x) ){
        return -1;
    }
    CarriedMass += x;
    if( CarriedMass < 0 ){
        CarriedMass = 0;
    }
    return CarriedMass;
}

int GetCarriedMass(){
    return CarriedMass ;
}

static int SetCarriedMass(int x){
    return CarriedMass = x;
}

int CalculateCarriedMass(){
    int ret;
    object *inv = all_inventory(this_object());
    if(sizeof(inv)){
        foreach(object ob in inv){
            ob->CalculateCarriedMass();
            ret += ob->GetMass();
        }
    }
    CarriedMass = ret;
    return ret;
}

int GetCarriedWeight(){
    object env;
    float h = DEFAULT_GRAVITY;

    if( env = environment() ){
        h = env->GetGravity();
    }
    return to_int( GetCarriedMass() * h );
}

int GetMaxCarry(){
    return MaxCarry;
}

int GetLivingMaxCarry(){
    return MaxCarry;
}

int SetMaxCarry(int x){
    return (MaxCarry = x);
}

int CanCarry(int amount){
    object env;
    float h;

    if( amount < 1 ) return 1;
    if( env = environment() ){
        if( !(living(env)) ){
            h = (float)env->GetGravity();
        }
        else {
            h = DEFAULT_GRAVITY;
        }
    }
    else {
        h = DEFAULT_GRAVITY;
    }
    if( ((GetCarriedMass() + amount) * h) > GetMaxCarry() ){
        return 0;
    }
    else {
        return 1;
    }
}
