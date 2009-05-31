#include <lib.h>
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
    tell_room(environment(victim), victimname+" looks suprised and grossed out.",({victim}) );
    tell_object(victim, "You are suprised and disgusted to realize that fleas are crawling around on you.");
    return 1;
}

void create(){
    germ::create();
    SetKeyName("tf1");
    SetId(({"tf2"}));
    SetLong("fleas version 1");
    SetCure(20);
    SetVendorType(VT_TREASURE);
    SetCommunicable(50);
    SetLifeSpan(666);
    SetType("parasite");
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


int eventSuffer(){
    int x;
    x=random(500);
    if(x < 5) damage1();
    else if(x < 10) damage2();
    else if(x < 15) damage3();
    else if(x < 20) damage4();
    return 1;
}

int DangerLevel(){
    if(victim->GetHealthPoints() < 20) return 100;
    return 1;
}

int FatigueLevel() {
    if(victim->GetStaminaPoints() < 11) return 10;
    return 11;
}

string GetAffectLong(object ob) {
    if(!ob || !living(ob)) return 0;
    return ob->GetName() + " looks uncomfortable and disheveled.";
}

int damage1(){
    tell_object(victim,"You feel really itchy all over.");
    tell_room(environment(victim),victimname+" looks uncomfortable.", ({victim}) );
    return 1;
}

int damage2(){
    tell_object(victim,"You involuntarily start scratching at the maddening itch.");
    tell_room(environment(victim),victimname+" scratches desperately at "+reflexive(victim)+".", ({victim}) );
    return 1;
}

int damage3(){
    tell_room(environment(victim),victimname+" scratches at "+reflexive(victim)+" in a frenzy, ripping "+
            possessive(victim)+" flesh and drawing blood.", ({victim}) );
    tell_object(victim,"You scratch at yourself in a mad frenzy, ripping flesh and drawing blood.");
    if(DangerLevel() != 100) victim->eventReceiveDamage(this_object(),DISEASE,random(5)+4,0,"torso");
    return 1;
}

int damage4(){
    tell_room(environment(victim),victimname+" looks disgusted.", ({victim}) );
    tell_object(victim,"You are disgusted as you feel fleas crawling on your body.");
    return 1;
}


mixed CanGet(object ob) { return "You just cant get rid of these parasites.";}
mixed CanDrop(object ob) { return "You just cant get rid of these parasites.";}

