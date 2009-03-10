#include <lib.h>
#include ROOMS_H
#include <vendor_types.h>
#include <damage_types.h>
inherit LIB_GERM;
int damage1();
int damage2();
int damage3();
int damage4();
int damage5();
int DangerLevel();
object victim,where;
string victimname;

int InfectMess(object ob){
    victim=ob;
    victimname=victim->GetName();
    tell_room(environment(victim), victimname+" looks pale and uncomfortable.",({victim}) );
    tell_object(victim, "You feel an unpleasant tickle in your throat.");
    return 1;
}

void create(){
    germ::create();
    SetKeyName("tc2");
    SetGermName("cold virus");
    SetId(({"tc2"}));
    SetLong("Test Cold version 2.1");
    SetCure(20);
    SetVendorType(VT_TREASURE);
    SetCommunicable(15);
    SetLifeSpan(1200);
    SetType("viral");
    SetInfect((: InfectMess :));
}

void bonuses(){
    if(victim && environment(this_object()) == victim){
        victim->AddStatBonus("intelligence", -5);
        victim->AddStatBonus("strength", -15);
        victim->AddStatBonus("charisma", -35);
        victim->AddStatBonus("durability", -15);
        victim->AddStatBonus("agility", -15);
        victim->AddStatBonus("coordination", -5);
        victim->AddStatBonus("speed", -5);
        victim->AddStatBonus("wisdom", -1);
    }
    else if(victim) {
        foreach( string stat in ({"intelligence", "strength",
                    "charisma", "durability", "agility", "coordination", "speed", "wisdom"})){
            victim->RemoveStatBonus(stat);
        }
    }
    return;
}


void init(){
    germ::init();
    bonuses();
}

int eventDestruct(){
    this_object()->eventMove(ROOM_FURNACE);
    return ::eventDestruct();
}

int eventMove(mixed dest){
    int ret = germ::eventMove(dest);
    if(environment() && !living(environment())) victim = 0;
    bonuses();
    return ret;
}

int eventSuffer(){
    int x;
    x=random(70);
    if(x < 8) environment()->eventForce("sneeze");
    else if(x < 5) damage1();
    else if(x < 10) damage2();
    else if(x < 15) damage3();
    else if(x < 20) damage4();
    else if(x < 2) damage5();
    return 1; 
}

int DangerLevel(){
    if(victim && victim->GetHealthPoints() < 100) return 100;
    return 1;
}

int FatigueLevel() {
    if(victim && victim->GetStaminaPoints() < 11) return 10;
    return 11;
}

string GetAffectLong(object ob) {
    if(!ob || !living(ob)) return 0;
    return ob->GetName() + " has a runny nose.\n"+ob->GetName()+" looks ill and disoriented.";
}

int damage1(){
    if(victim){
        tell_object(victim,"You feel miserable.");
        tell_room(environment(victim),victimname+" looks miserable.", ({victim}) );
        if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-5)-5);
    }
    return 1;
}

int damage2(){
    if(victim){
        tell_object(victim,"You are racked by a fit of wet-sounding coughs.");
        tell_room(environment(victim),victimname+" is racked by a fit of wet-sounding coughs.", ({victim}) );
        if(DangerLevel() != 100) victim->AddHP(-(random(10)+10));
        if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-10)-5);
    }
    return 1;
}

int damage3(){
    if(victim){
        tell_room(environment(victim),victimname+" lets out a pathetic little groan of discomfort.", ({victim}) );
        tell_object(victim,"You let out a little groan of discomfort as a wave of weakness hits you.");
        if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-10)-10);
    }
    return 1;
}

int damage4(){
    if(victim){
        tell_room(environment(victim),victimname+" sneezes violently, and then whimpers.", ({victim}) );
        tell_object(victim,"You sneeze violently, and it seems as though your head is composed of pain.");
        if(DangerLevel() != 100) victim->AddHP(-(random(10)+2));
        if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-15)-10);
    }
    return 1;
}

int damage5(){
    if(victim){
        tell_room(environment(victim),victimname+" makes a horrible, gurgling, coughing noise and hacks up "+
                "a huge glob of phlegm.", ({victim}) );
        tell_object(victim,"You cough up a huge glob of phlegm, accidentally inhale it, and choke while coughing it "+
                "back up.");
        if(DangerLevel() != 100) victim->AddHP(-(random(20)+10));
        if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-15)-15);
    }
    return 1;
}


mixed CanGet(object ob) { return "Your fingers slip on your runny snot.";}
mixed CanDrop(object ob) { return "Your fingers slip on your runny snot.";}
