/*    /lib/std/limb.c
 *    From the Dead Souls Mud Library
 *    Standard limb object for severed limbs
 *    Created by Descartes of Borg 950329
 *    Version: @(#) limb.c 1.5@(#)
 *    Last Modified: 96/12/31
 */

#include <lib.h>
#include <medium.h>
#include <respiration_types.h>

inherit LIB_SURFACE;

int Count          = 0;
int Player         = 0;
string Limb        = 0;
string Owner       = 0;
string Race        = 0;
int CallOut = -1;
int stank, nodecay, slowdecay;

int eventDecay();

string GetLimb(){
    return Limb;
}
string GetItemCondition(){
    return "";
}

void create(){
    surface::create();
}

void init(){
    surface::init();
}

void SetLimb(string limb, string owner, string race){
    string justlimb;
    sscanf(limb,"%*s %s",justlimb);
    SetKeyName(limb);
    SetId( ({ limb, "flesh","pile","limb",(justlimb||"chunk") }) );
    SetAdjectives(({"severed","rotting",race}));
    Limb = limb;
    Owner = owner;
    Race = race;
    Player = interactive(previous_object());
    Count = 1;
    SetShort("a rotting " + possessive_noun(Race) + " " + Limb);
    SetLong("This limb has a horrible stench as it rots to nothing.");
    if(Player){
        SetNoClean(1);
        slowdecay = 50;
    }
}

string GetOwner(){
    return Owner;
}

string GetRace(){
    return Race;
}

int GetSaveString(){
    return 0;
}

string GetShort(){
    string str = surface::GetShort();

    if( !str ){
        str = "a limb";
    }
    return str;
}

int CanReceive(object ob){
    return 1;
}

int Destruct(){
    return surface::Destruct();
}

int eventDecay(){
    int medium, rtype;
    if( !room_environment() ){
        Destruct();
        return 0;
    }
    medium = room_environment()->GetMedium();
    rtype = room_environment()->GetRespirationType();
    if(rtype & R_AIR) stank = 1;
    if(medium == MEDIUM_SPACE && random(100) < 90) return 0;
    switch(Count){
        case 10:
            if(stank)
                message("smell", "The "+Limb+" really stinks.", environment());
            SetShort("the stinky remains of a rotting " + Limb);
            break;
        case 20:
            if(stank)
                message("smell", "A rotting stench fills the entire area.",
                        environment());
            SetShort("a pile of rotting flesh");
            break;
        case 30:
            Destruct();
            return 0;
    }
    if(slowdecay){
        if(random(100) > slowdecay) Count++;
    }
    else Count++;
    return Count;
}

int SetNoDecay(int i){
    if(i) nodecay = 1;
    else nodecay = 0;
    return nodecay;
}

int GetNoDecay(){
    return nodecay;
}

int SetSlowDecay(int i){
    if(i) slowdecay = i;
    else slowdecay = 0;
    return slowdecay;
}

int GetSlowDecay(){
    return slowdecay;
}

int SetCount(int i){
    Count = i;
    return Count;
}

int GetCount(int i){
    return Count;
}

