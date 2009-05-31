#include <lib.h>
#include <vendor_types.h>
#include <damage_types.h>
#include <position.h>
#include <medium.h>
#include <daemons.h>

inherit LIB_MISSILE;

int detonated;
int armed;
string *limbs;
void analyze(string butt);
int HitLivings(object munch);
int radius = 0;
object *whom;

void create(){
    ::create();
    SetKeyName("torpedo");
    SetId( ({"torpedo","mk14","mark 14","cylinder"}) );
    SetAdjectives( ({"electric","mk14", "mark 14"}) );
    SetShort("a pink torpedo");
    SetLong("Typical of early models, this torpedo was produced during a "+
            "severe shortage of red and white lead undercoat pigments, requiring "+
            "its pink anti-friction paint. This self-propelled cylinder is "+
            "twenty feet long and packed with a tremendous explosive charge. "+
            "It has markings stenciled on the side.");
    SetCruiseInMessage("A torpedo cruises in!");
    SetCruiseOutMessage("The torpedo cruises $D.");
    SetMass(33000);
    SetDollarCost(10000);
    SetRange(200);
    SetSpeed(3);
    SetVendorType(VT_WEAPON);
    AddItem(({"motor","electric motor"}),"The thing that pushes this torpedo.");
    detonated=0;
    SetItems( ([
                ({"markings","marking","stencil"}) : "Markings "+
                "you can read, applied in stencil.",
                ({"fins","flaps"}) : "This baby's got 'em.",
                ]) );
    SetReads( ([
                "default" : "Try 'read markings on torpedo'",
                ({"markings","marking","stencil"}) : "SS-666 "+
                "Sea Tiger",
                ]) );
}

int eventSink(){
    if(!armed) return ::eventSink();
    return 0;
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
    if(detonated==1) this_object()->eventDestruct();
    if(interactive(environment())){
        tell_object(environment(), "You are holding a live torpedo.");
        return;
    }
    tell_object(environment(),"You hear a loud whining sound.");
    tell_object(environment(environment()), environment()->GetName()+" is holding a live torpedo.");
}

int detonate(){
    object ob;
    string *stuffs;
    int num;
    ob = environment(this_object());
    if(living(ob) && !(RACES_D->GetNonMeatRace(ob->GetRace()))){
        stuffs=ob->GetLimbs();
        tell_object(ob, "\nKABOOM! You are torn to pieces by a torpedo!\n");
        tell_room(environment(ob), "\nKABOOM! "+ob->GetName()+
                " is torn to pieces by a torpedo!\n",ob);
        detonated=2;
        foreach(string limb in stuffs){
            if(limb != "head" && limb != "torso" && limb != "neck") ob->RemoveLimb(limb,this_object());
        }
        ob = environment(ob);
    }
    if(detonated !=2) tell_room(environment(this_object()), 
            "\nKABOOM! A torpedo detonates!\n");
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
    int medium;
    if(!env) return 1;
    if(!inherits(LIB_ROOM,env)){
        detonate();
        return 0;
    }
    medium = env->GetMedium();
    if(medium != MEDIUM_SURFACE && medium != MEDIUM_WATER 
            && medium != MEDIUM_AIR && medium != MEDIUM_SPACE){
        detonate();
        return 0;
    }
    foreach(object ob in all_inventory(env)){
        if(ob == this_object() || ob->isDummy()) continue;
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

int eventCruise(string str){
    object env = environment();
    if(env && env->GetMedium() == MEDIUM_SPACE){
        return 1;
    }
    else return ::eventCruise(str);
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
    if(radius < 6) dam = random(10000)+5000;
    else dam = (random(10000)+5000) - (radius * 1000);
    if(env && living(env)){
        dam *= 5;
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

mixed eventRunOut(){
    return detonate();
}

