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
    tell_room(environment(victim), victimname+" looks suprised and grossed out.",({victim}) );
    tell_object(victim, "You are suprised and disgusted to realize that lice are crawling around on you.");
    return 1;
}

void create(){
    germ::create();
    SetKeyName("tf1");
    SetId(({"tf2"}));
    SetLong("lice version 1");
    SetCure(20);
    SetVendorType(VT_TREASURE);
    SetCommunicable(50);
    SetLifeSpan(2000);
    SetType("parasite");
    SetInfect((: InfectMess :));
}

void bonuses(){
    object player;
    if(!player=victim) return;
    player->AddStatBonus("intelligence", -1);
    player->AddStatBonus("charisma", -35);
    player->AddStatBonus("durability", -5);
    player->AddStatBonus("agility", -5);
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
    if(x < 25) damage4();
    else if(x < 50) damage2();
    else if(x < 85) damage3();
    else if(x < 90) damage4();
    if(!busy){
	busy=1;
	call_out("reset",60);
    }
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
    tell_room(environment(victim),victimname+" scratches desperately at "+objective(this_player())+"self.", ({victim}) );
    return 1;
}

int damage3(){
    tell_room(environment(victim),victimname+" scratches at "+objective(this_player())+"self in a frenzy, ripping "+
      possessive(this_player())+" flesh and drawing blood.", ({victim}) );
    tell_object(victim,"You scratch at yourself in a mad frenzy, ripping flesh and drawing blood.");
    if(DangerLevel() != 100) victim->eventReceiveDamage(this_object(),DISEASE,random(5)+4,0,"torso");
    return 1;
}

int damage4(){
    tell_room(environment(victim),victimname+" looks disgusted.", ({victim}) );
    tell_object(victim,"You are disgusted as you feel lice crawling on your body.");
    return 1;
}


mixed CanGet(object ob) { return "Your fingers slip on your runny snot.";}
mixed CanDrop(object ob) { return "Your fingers slip on your runny snot.";}

