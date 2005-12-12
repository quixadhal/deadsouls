#include <lib.h>
#include <vendor_types.h>
#include <damage_types.h>
inherit "/lib/std/germ";
int damage1();
int damage2();
int damage3();
int damage4();
int damage5();
int DangerLevel();
object victim,disease,where;
string victimname;
int busy;

void whoami(){
    disease=this_object();
    return;
}

int InfectMess(object ob){
    victim=ob;
    victimname=victim->GetName();
    tell_room(environment(victim), victimname+" looks pale and uncomfortable.",({victim}) );
    tell_object(victim, "Your feel an unpleasant tickle in your throat.");
    return 1;
}

void create(){
    germ::create();
    SetKeyName("tc2");
    SetId(({"tc1"}));
    SetLong("Test Cold version 2.1");
    SetCure(20);
    SetVendorType(VT_TREASURE);
    SetCommunicable(50);
    SetLifeSpan(2000);
    SetType("viral");
    SetInfect((: InfectMess :));
    //set_heart_beat(8);
}

void bonuses(){
    object player;
    if(!player=victim) return;
    player->AddStatBonus("intelligence", -5);
    player->AddStatBonus("strength", -15);
    player->AddStatBonus("charisma", -35);
    player->AddStatBonus("durability", -15);
    player->AddStatBonus("agility", -15);
    player->AddStatBonus("coordination", -5);
    player->AddStatBonus("speed", -5);
    player->AddStatBonus("wisdom", -1);
    return;
}


void init(){
    object player;
    germ::init();
    germ::heart_beat();
    whoami();
    bonuses();
}


void reset(){
    busy=0;
    init();
    whoami();
}

int eventSuffer(){
    int x;
    whoami();
    x=random(100);
    if(x < 2) environment()->eventForce("sneeze");
    else if(x < 5) damage1();
    else if(x < 10) damage2();
    else if(x < 15) damage3();
    else if(x < 20) damage4();
    else if(x > 98) damage5();
    if(!busy){
	busy=1;
	call_out("reset",60);
    }
    return 1;
}

int DangerLevel(){
    if(victim->GetHealthPoints() < 100) return 100;
    return 1;
}

int FatigueLevel() {
    if(victim->GetStaminaPoints() < 11) return 10;
    return 11;
}

string GetAffectLong(object ob) {
    if(!ob || !living(ob)) return 0;
    return ob->GetName() + " has a runny nose.\n"+ob->GetName()+" looks ill and disoriented.";
}

int damage1(){
    tell_object(victim,"You feel miserable.");
    tell_room(environment(victim),victimname+" looks miserable.", ({victim}) );
    if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-5)-5);
    return 1;
}

int damage2(){
    tell_object(victim,"You are racked by a fit of wet-sounding coughs.");
    tell_room(environment(victim),victimname+" is racked by a fit of wet-sounding coughs.", ({victim}) );
    if(DangerLevel() != 100) victim->eventReceiveDamage(this_object(),DISEASE,random(10)+10,0,"torso");
    if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-10)-5);
    return 1;
}

int damage3(){
    tell_room(environment(victim),victimname+" lets out a pathetic little groan of discomfort.", ({victim}) );
    tell_object(victim,"You let out a little groan of discomfort as a wave of weakness hits you.");
    if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-10)-10);
    return 1;
}

int damage4(){
    tell_room(environment(victim),victimname+" sneezes violently, and then whimpers.", ({victim}) );
    tell_object(victim,"You sneeze violently, and it seems as though your head is composed of pain.");
    if(DangerLevel() != 100) victim->eventReceiveDamage(this_object(),DISEASE,random(10)+2,0,"head");
    if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-15)-10);
    return 1;
}

int damage5(){
    tell_room(environment(victim),victimname+" makes a horrible, gurgling, coughing noise and hacks up "+
      "a huge glob of phlegm.", ({victim}) );
    tell_object(victim,"You cough up a huge glob of phlegm, accidentally inhale it, and choke while coughing it "+
      "back up.");
    if(DangerLevel() != 100) victim->eventReceiveDamage(this_object(),DISEASE,random(20)+10,0,"head");
    if(FatigueLevel() > 10) victim->AddStaminaPoints(random(-15)-15);
    return 1;
}


mixed CanGet(object ob) { return "Your fingers slip on your runny snot.";}
mixed CanDrop(object ob) { return "Your fingers slip on your runny snot.";}

