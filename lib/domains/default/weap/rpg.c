#include <lib.h>
#include <vendor_types.h>
#include <damage_types.h>
#include <position.h>

inherit LIB_MISSILE;

int detonated;
int count, armed;
string *limbs,domain;
void analyze(string butt);
int HitLivings(object munch);
int radius = 0;
object *whom;

void create(){
    ::create();
    SetKeyName("rocket propelled grenade");
    sscanf(base_name(this_object()),"/domains/%s/%*s",domain);
    SetId( ({"grenade","rpg"}) );
    SetAdjectives( ({"rocket","propelled","angular","oblong"}) );
    SetShort("a rocket propelled grenade");
    SetLong("This is an angular, oblong device attached to a tubelike rocket motor. "+
            "It is usually installed in a rocket propelled grenade launcher and shot "+
            "at faraway enemies.");
    SetCruiseInMessage("A rocket propelled grenade roars in!");
    SetCruiseOutMessage("The rocket propelled grenade flies $D.");
    SetMass(20);
    SetDollarCost(10);
    SetRange(20);
    SetSpeed(5);
    SetVendorType(VT_WEAPON);
    AddItem(({"motor","rocket motor"}),"The thing that pushes this grenade.");
    count=5;
    detonated=0;
}

int eventFall(){
    if(!armed) return ::eventFall();
    return 0;
}

void init(){
    ::init();
    if(armed){
        set_heart_beat(1);
        if(detonated==1) this_object()->eventDestruct();
    }
}

void heart_beat(){
    count--;
    if(detonated==1) this_object()->eventDestruct();
    if(count < 0){
        this_object()->detonate();
        return;
    }
    if(interactive(environment())){
        tell_object(environment(), "You are holding a live rocket propelled grenade.");
        return;
    }
    tell_object(environment(),"You hear a low hissing sound.");
    tell_object(environment(environment()), environment()->GetName()+" is holding a live rocket propelled grenade.");
}

int detonate(){
    object ob;
    string *stuffs;
    int num;
    ob = environment(this_object());
    if(living(ob)){
        stuffs=ob->GetLimbs();
        tell_object(ob, "\nKABOOM! You are torn to pieces by a rocket propelled grenade!\n");
        tell_room(environment(ob), "\nKABOOM! "+ob->GetName()+" is torn to pieces by "+
                " a rocket propelled grenade!\n",ob);
        detonated=2;
        foreach(string limb in stuffs){
            if(limb != "head" && limb != "torso" && limb != "neck") ob->RemoveLimb(limb,this_object());
        }
        ob = environment(ob);
    }
    if(detonated !=2) tell_room(environment(this_object()), "\nKABOOM! A rocket "+
            "propelled grenade detonates!\n");
    if(!sizeof(get_livings(ob))) ob = environment(ob);
    if(ob && sizeof(whom = scramble_array(get_livings(ob)[0..12]))){
        foreach(object victim in whom){
            this_object()->HitLivings(victim);
        }
    }
    this_object()->eventDestruct();
    return 1;
}

mixed eventNegotiateObstacles(){
    object env = environment();
    if(!env) return 1;
    if(!inherits(LIB_ROOM,env)){
        detonate();
        return 0;
    }
    foreach(object ob in all_inventory(env)){
        if(living(ob) && ob->GetPosition() != POSITION_LYING){
            if(random(100) > 20){
                this_object()->eventMove(ob);
                detonate();
                return 0;
            }
        }
        else if(ob->GetMass() > 1000){
            if(random(100) > 30){
                detonate();
                return 0;
            }
        }
    }
    return ::eventNegotiateObstacles();
}

mixed eventEncounterBlock(){
    detonate();
    return ::eventEncounterBlock();
}


void analyze(string str){
    string tmp,tmp2;
    sscanf(str,"%s#%s",tmp,tmp2);
    if(tmp="/lib/corpse"){
        filter(deep_inventory(find_object(str)), (: $1->eventDestruct() :), this_object() );
    }
    return;
}

int HitLivings(object ob){
    object env = environment();
    int dam;
    radius++;
    if(radius < 6) dam = random(1000)+500;
    else dam = (random(1000)+500) - (radius * 100);
    if(env && !living(env)){
        if(env->GetClimate() == "indoors") dam *= 2;
    }
    if(dam > 0){
        ob->eventReceiveDamage("concussion",OVERPRESSURE, dam, 0);
    }
    return 1;
}

int SetArmed(int x){
    if(x) armed = 1;
    else armed = 0;
    if(armed) set_heart_beat(1);
    return armed;
}
