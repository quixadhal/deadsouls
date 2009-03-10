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
    tell_room(environment(victim), victimname+" looks weak and woozy.",({victim}) );
    tell_object(victim, "You feel weak and dizzy.");
    return 1;
}

void create(){
    germ::create();
    SetKeyName("tf2");
    SetGermName("flu virus");
    SetId(({"tf2"}));
    SetLong("Test Flu version 2");
    SetCure(20);
    SetCommunicable(15);
    SetLifeSpan(900);
    SetType("viral");
    SetInfect((: InfectMess :));
}

void bonuses(){
    if(victim && environment(this_object()) == victim){
        victim->AddStatBonus("intelligence", -15);
        victim->AddStatBonus("strength", -25);
        victim->AddStatBonus("charisma", -45);
        victim->AddStatBonus("durability", -25);
        victim->AddStatBonus("agility", -25);
        victim->AddStatBonus("coordination", -15);
        victim->AddStatBonus("speed", -15);
        victim->AddStatBonus("wisdom", -2);
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
    x=random(500);
    if(x < 2) environment()->eventForce("sneeze");
    else if(x < 5) damage1();
    else if(x < 10) damage2();
    else if(x < 15) damage3();
    else if(x < 20) damage4();
    else if(x > 98) damage5();
    return 1;
}

int DangerLevel(){
    if(victim && victim->GetHealthPoints() < 50) return 100;
    return 1;
}

int FatigueLevel() {
    if(victim && victim->GetStaminaPoints() < 11) return 10;
    return 11;
}

string GetAffectLong(object ob) {
    if(!ob || !living(ob)) return 0;
    return ob->GetName() + " is disheveled.\n"+ob->GetName()+" looks very ill and disoriented.";
}

int damage1(){
    if(victim){
        tell_object(victim,"You feel weak and ill.");
        tell_room(environment(victim),victimname+" looks pale and ill.", ({victim}) );
        if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-15)-15);
    }
    return 1;
}

int damage2(){
    if(victim){
        tell_object(victim,"You are racked by a fit of gruesome-sounding, hacking coughs.");
        tell_room(environment(victim),victimname+" is racked by a fit of gruesome-sounding, hacking coughs.", ({victim}) );
        if(DangerLevel() != 100) victim->AddHP(-(random(20)+10));
        if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-10)-5);
    }
    return 1;
}

int damage3(){
    if(victim){
        tell_room(environment(victim),victimname+" lets out a groan of discomfort.", ({victim}) );
        tell_object(victim,"You let out a groan of discomfort as a wave of weakness hits you.");
        if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-10)-10);
    }
    return 1;
}

int damage4(){
    if(victim){
        tell_room(environment(victim),victimname+" gags violently, then chokes out a thick rope of vomit onto the ground.", ({victim}) );
        tell_object(victim,"You gag violently, then choke out a thick rope of vomit onto the ground.");
        if(DangerLevel() != 100) victim->AddHP(-(random(30)+15));
        if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-25)-20);
    }
    return 1;
}

int damage5(){
    if(victim){
        if(victim->GetPosition() != 1){
            tell_room(environment(victim),victimname+" makes a horrendous flatulent noise and falls helplessly to the floor, soiling "+objective(victim)+"self.", ({victim}) );
            tell_object(victim,"You make a horrendous flatulent noise and fall helplessly to the floor, soiling yourself.");
            victim->SetPosition(1);
        }
        if(victim->GetPosition() == 1){
            tell_room(environment(victim),victimname+" makes a horrendous flatulent noise as "+nominative(victim)+" lies helplessly on the ground.", ({ victim}));
            tell_object(victim,"You make a horrendous flatulent noise as you lie helplessly on the ground.");
        }
        if(DangerLevel() != 100) victim->AddHP(-(random(35)+15));
        if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-35)-25);
    }
    return 1;
}


mixed CanGet(object ob) { return "Your fingers slip on your runny snot.";}
mixed CanDrop(object ob) { return "Your fingers slip on your runny snot.";}
